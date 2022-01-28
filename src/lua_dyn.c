
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP_
#include <omp.h>
#endif

#include "lua.h"
#include "lauxlib.h"

#ifdef WIN32
#define LAZYAPICALL     //__stdcall
#include <windows.h>
#else
#define LAZYAPICALL
#endif
#define LAZYPRECALL     static
#define GModule         void
#define gpointer        void
#define bool            int



/* Static member of this file */
void *lzy_lua_load_library(char **);
void lzy_lua_unload_library();
static void lzy_lua_unload_symbols();
static bool lzy_lua_debug_enabled();
static int default_failure_callback(const char *symbol);
static int load_symbol_or_err(const char *name, gpointer *symbol);

/* Handle to the library */
static GModule *module = NULL;
#ifdef _DEBUG
static int debug_enabled=1;
#else
static int debug_enabled=0;
#endif
static int nenvptr=0;

/* Debugging macros */
static int PRINT_DEBUG(const char *szfunc, const char *fmt, ...) {
  va_list args;
  FILE *ofp = stderr;
  int out=0;
  if (debug_enabled || lzy_lua_debug_enabled()) {
    fprintf(ofp,"\n%s: ",szfunc);
    va_start(args, fmt);
    out = vfprintf(ofp, fmt, args);
    va_end(args);
    fflush(ofp);
  }
  return out;
}

static int PRINT_ERR(const char *szfunc, const char *fmt, ...) {
  va_list args;
  FILE *ofp = stderr;
  int out=0;
  {
    fprintf(ofp,"\n%s: ",szfunc);
    va_start(args, fmt);
    out = vfprintf(ofp, fmt, args);
    va_end(args);
    fflush(ofp);
  }
  return out;
}

/* Called whenever the library is about to crash */
static int (*failure_callback)(const char *err) = default_failure_callback;

/* True if the environment variable DEBUG_LUA_DYNLOADER is non empty */
bool lzy_lua_debug_enabled() {
    const char *s = getenv("DEBUG_LUA_DYNLOADER");
    return (s != NULL && strlen(s) > 0);
}

/* Prints the failing symbol and aborts */
static int default_failure_callback(const char *symbol){
    PRINT_ERR (__FUNCTION__,"the symbol %s could not be found!", symbol);
    return(1);
}

/* unload the library */
void lzy_lua_unload_library(){
    int ierr;
	  lzy_lua_unload_symbols();
    if (module) {
#ifdef WIN32
      ierr = FreeLibrary(module);
      ierr = !ierr; //negate Bill's sick ways
#else
      ierr = dlclose(module);
#endif
      if (!ierr)
        module=NULL;
    }
    return;
}
 
/* Searches and loads the actual library */
void *lzy_lua_load_library(char **libnames){
    int i;
    PRINT_DEBUG(__FUNCTION__,"Looking for a suitable library.");
    for (i = 0; libnames[i] != NULL; i++){
        if (module != NULL)
            break;
        PRINT_DEBUG(__FUNCTION__,"Trying to load %s...", libnames[i]);
#ifdef WIN32
        module = (void*) LoadLibrary(TEXT(libnames[i]));
#else
        module = (void*) dlopen(libnames[i],RTLD_NOW);
#endif
        //module = g_module_open(libnames[i], G_MODULE_BIND_LAZY|G_MODULE_BIND_LOCAL);
    }
    if (module == NULL)
        PRINT_DEBUG(__FUNCTION__,"Library lookup failed! Check the manual to customize the lookup.");
    else
        PRINT_DEBUG(__FUNCTION__,"Success! Handle:%p",module);
    return module;
}

static void* g_module_symbol(void *hlib, const char *name, void **fun) {
#ifdef WIN32
  *fun = GetProcAddress(hlib, name);
#else
  *fun = dlsym(hlib, name);
#endif
    return *fun;
}

static int load_symbol_or_err(const char *name, gpointer *symbol){
    int retval = 0;
    if (!module || !g_module_symbol(module, name, symbol))
        retval = failure_callback(name);
    else
        retval = PRINT_DEBUG(__FUNCTION__,"successfully imported the symbol %s.", name);
    return retval;
}

/* imported functions lua.h */

 LUA_API lua_State *(LAZYAPICALL *native_lua_newstate) (lua_Alloc f, void *ud) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_close) (lua_State *L) = NULL;
 LUA_API lua_State *(LAZYAPICALL *native_lua_newthread) (lua_State *L) = NULL;
 LUA_API lua_CFunction (LAZYAPICALL *native_lua_atpanic) (lua_State *L, lua_CFunction panicf) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_gettop) (lua_State *L) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_settop) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushvalue) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_remove) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_insert) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_replace) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_checkstack) (lua_State *L, int sz) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_xmove) (lua_State *from, lua_State *to, int n) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_isnumber) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_isstring) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_iscfunction) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_isuserdata) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_type) (lua_State *L, int idx) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_typename) (lua_State *L, int tp) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_equal) (lua_State *L, int idx1, int idx2) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_rawequal) (lua_State *L, int idx1, int idx2) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_lessthan) (lua_State *L, int idx1, int idx2) = NULL;
 LUA_API lua_Number (LAZYAPICALL *native_lua_tonumber) (lua_State *L, int idx) = NULL;
 LUA_API lua_Integer (LAZYAPICALL *native_lua_tointeger) (lua_State *L, int idx) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_toboolean) (lua_State *L, int idx) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_tolstring) (lua_State *L, int idx, size_t *len) = NULL;
 LUA_API size_t (LAZYAPICALL *native_lua_objlen) (lua_State *L, int idx) = NULL;
 LUA_API lua_CFunction (LAZYAPICALL *native_lua_tocfunction) (lua_State *L, int idx) = NULL;
 LUA_API void *(LAZYAPICALL *native_lua_touserdata) (lua_State *L, int idx) = NULL;
 LUA_API lua_State *(LAZYAPICALL *native_lua_tothread) (lua_State *L, int idx) = NULL;
 LUA_API const void *(LAZYAPICALL *native_lua_topointer) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushnil) (lua_State *L) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushnumber) (lua_State *L, lua_Number n) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushinteger) (lua_State *L, lua_Integer n) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushlstring) (lua_State *L, const char *s, size_t l) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushstring) (lua_State *L, const char *s) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_pushvfstring) (lua_State *L, const char *fmt, va_list argp) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_pushfstring) (lua_State *L, const char *fmt, ...) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushboolean) (lua_State *L, int b) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_pushlightuserdata) (lua_State *L, void *p) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_pushthread) (lua_State *L) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_gettable) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_getfield) (lua_State *L, int idx, const char *k) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_rawget) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_rawgeti) (lua_State *L, int idx, int n) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_createtable) (lua_State *L, int narr, int nrec) = NULL;
 LUA_API void *(LAZYAPICALL *native_lua_newuserdata) (lua_State *L, size_t sz) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_getmetatable) (lua_State *L, int objindex) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_getfenv) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_settable) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_setfield) (lua_State *L, int idx, const char *k) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_rawset) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_rawseti) (lua_State *L, int idx, int n) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_setmetatable) (lua_State *L, int objindex) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_setfenv) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_call) (lua_State *L, int nargs, int nresults) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_pcall) (lua_State *L, int nargs, int nresults, int errfunc) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_cpcall) (lua_State *L, lua_CFunction func, void *ud) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_load) (lua_State *L, lua_Reader reader, void *dt, const char *chunkname) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_dump) (lua_State *L, lua_Writer writer, void *data) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_yield) (lua_State *L, int nresults) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_resume) (lua_State *L, int narg) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_status) (lua_State *L) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_gc) (lua_State *L, int what, int data) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_error) (lua_State *L) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_next) (lua_State *L, int idx) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_concat) (lua_State *L, int n) = NULL;
 LUA_API lua_Alloc (LAZYAPICALL *native_lua_getallocf) (lua_State *L, void **ud) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_setallocf) (lua_State *L, lua_Alloc f, void *ud) = NULL;
 LUA_API void (LAZYAPICALL *native_lua_setlevel) (lua_State *from, lua_State *to) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_getstack) (lua_State *L, int level, lua_Debug *ar) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_getinfo) (lua_State *L, const char *what, lua_Debug *ar) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_getlocal) (lua_State *L, const lua_Debug *ar, int n) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_setlocal) (lua_State *L, const lua_Debug *ar, int n) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_getupvalue) (lua_State *L, int funcindex, int n) = NULL;
 LUA_API const char *(LAZYAPICALL *native_lua_setupvalue) (lua_State *L, int funcindex, int n) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_sethook) (lua_State *L, lua_Hook func, int mask, int count) = NULL;
 LUA_API lua_Hook (LAZYAPICALL *native_lua_gethook) (lua_State *L) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_gethookmask) (lua_State *L) = NULL;
 LUA_API int (LAZYAPICALL *native_lua_gethookcount) (lua_State *L) = NULL;

/* imported functions from lauxlib.h */

 LUALIB_API lua_State *(LAZYAPICALL *native_luaL_newstate) (void) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaI_openlib) (lua_State *L, const char *libname, const luaL_Reg *l, int nup) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_register) (lua_State *L, const char *libname, const luaL_Reg *l) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_getmetafield) (lua_State *L, int obj, const char *e) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_callmeta) (lua_State *L, int obj, const char *e) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_typerror) (lua_State *L, int narg, const char *tname) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_argerror) (lua_State *L, int numarg, const char *extramsg) = NULL;
 LUALIB_API const char *(LAZYAPICALL *native_luaL_checklstring) (lua_State *L, int numArg, size_t *l) = NULL;
 LUALIB_API const char *(LAZYAPICALL *native_luaL_optlstring) (lua_State *L, int numArg, const char *def, size_t *l) = NULL;
 LUALIB_API lua_Number (LAZYAPICALL *native_luaL_checknumber) (lua_State *L, int numArg) = NULL;
 LUALIB_API lua_Number (LAZYAPICALL *native_luaL_optnumber) (lua_State *L, int nArg, lua_Number def) = NULL;
 LUALIB_API lua_Integer (LAZYAPICALL *native_luaL_checkinteger) (lua_State *L, int numArg) = NULL;
 LUALIB_API lua_Integer (LAZYAPICALL *native_luaL_optinteger) (lua_State *L, int nArg, lua_Integer def) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_checkstack) (lua_State *L, int sz, const char *msg) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_checktype) (lua_State *L, int narg, int t) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_checkany) (lua_State *L, int narg) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_newmetatable) (lua_State *L, const char *tname) = NULL;
 LUALIB_API void *(LAZYAPICALL *native_luaL_checkudata) (lua_State *L, int ud, const char *tname) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_where) (lua_State *L, int lvl) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_error) (lua_State *L, const char *fmt, ...) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_checkoption) (lua_State *L, int narg, const char *def, const char *const lst[]) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_ref) (lua_State *L, int t) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_unref) (lua_State *L, int t, int ref) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_loadfile) (lua_State *L, const char *filename) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_loadbuffer) (lua_State *L, const char *buff, size_t sz, const char *name) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaL_loadstring) (lua_State *L, const char *s) = NULL;
 LUALIB_API const char *(LAZYAPICALL *native_luaL_gsub) (lua_State *L, const char *s, const char *p, const char *r) = NULL;
 LUALIB_API const char *(LAZYAPICALL *native_luaL_findtable) (lua_State *L, int idx, const char *fname, int szhint) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_buffinit) (lua_State *L, luaL_Buffer *B) = NULL;
 LUALIB_API char *(LAZYAPICALL *native_luaL_prepbuffer) (luaL_Buffer *B) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_addlstring) (luaL_Buffer *B, const char *s, size_t l) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_addstring) (luaL_Buffer *B, const char *s) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_addvalue) (luaL_Buffer *B) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_pushresult) (luaL_Buffer *B) = NULL;


 /* imported functions lualib.h */

 LUALIB_API int (LAZYAPICALL *native_luaopen_base) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_table) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_io) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_os) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_string) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_math) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_debug) (lua_State *L) = NULL;
 LUALIB_API int (LAZYAPICALL *native_luaopen_package) (lua_State *L) = NULL;
 LUALIB_API void (LAZYAPICALL *native_luaL_openlibs) (lua_State *L) = NULL;

/* end of hijacked functions */

/* reset all functions  */

static void lzy_lua_unload_symbols() {

  native_lua_newstate = NULL;
  native_lua_close = NULL;
  native_lua_newthread = NULL;
  native_lua_gettop = NULL;
  native_lua_settop = NULL;
  native_lua_pushvalue = NULL;
  native_lua_remove = NULL;
  native_lua_insert = NULL;
  native_lua_replace = NULL;
  native_lua_checkstack = NULL;
  native_lua_xmove = NULL;
  native_lua_isnumber = NULL;
  native_lua_isstring = NULL;
  native_lua_iscfunction = NULL;
  native_lua_isuserdata = NULL;
  native_lua_type = NULL;
  native_lua_typename = NULL;
  native_lua_equal = NULL;
  native_lua_rawequal = NULL;
  native_lua_lessthan = NULL;
  native_lua_tonumber = NULL;
  native_lua_tointeger = NULL;
  native_lua_toboolean = NULL;
  native_lua_tolstring = NULL;
  native_lua_objlen = NULL;
  native_lua_tocfunction = NULL;
  native_lua_touserdata = NULL;
  native_lua_tothread = NULL;
  native_lua_topointer = NULL;
  native_lua_pushnil = NULL;
  native_lua_pushnumber = NULL;
  native_lua_pushinteger = NULL;
  native_lua_pushlstring = NULL;
  native_lua_pushstring = NULL;
  native_lua_pushvfstring = NULL;
  native_lua_pushfstring = NULL;
  native_lua_pushcclosure = NULL;
  native_lua_pushboolean = NULL;
  native_lua_pushlightuserdata = NULL;
  native_lua_pushthread = NULL;
  native_lua_gettable = NULL;
  native_lua_getfield = NULL;
  native_lua_rawget = NULL;
  native_lua_rawgeti = NULL;
  native_lua_createtable = NULL;
  native_lua_newuserdata = NULL;
  native_lua_getmetatable = NULL;
  native_lua_getfenv = NULL;
  native_lua_settable = NULL;
  native_lua_setfield = NULL;
  native_lua_rawset = NULL;
  native_lua_rawseti = NULL;
  native_lua_setmetatable = NULL;
  native_lua_setfenv = NULL;
  native_lua_call = NULL;
  native_lua_pcall = NULL;
  native_lua_cpcall = NULL;
  native_lua_load = NULL;
  native_lua_dump = NULL;
  native_lua_yield = NULL;
  native_lua_resume = NULL;
  native_lua_status = NULL;
  native_lua_gc = NULL;
  native_lua_error = NULL;
  native_lua_next = NULL;
  native_lua_concat = NULL;
  native_lua_getallocf = NULL;
  native_lua_setallocf = NULL;
  native_lua_setlevel = NULL;
  native_lua_getstack = NULL;
  native_lua_getinfo = NULL;
  native_lua_getlocal = NULL;
  native_lua_setlocal = NULL;
  native_lua_getupvalue = NULL;
  native_lua_setupvalue = NULL;
  native_lua_sethook = NULL;
  native_lua_gethook = NULL;
  native_lua_gethookmask = NULL;
  native_lua_gethookcount = NULL;
  native_luaL_newstate = NULL;
  native_luaI_openlib = NULL;
  native_luaL_register = NULL;
  native_luaL_getmetafield = NULL;
  native_luaL_callmeta = NULL;
  native_luaL_typerror = NULL;
  native_luaL_argerror = NULL;
  native_luaL_checklstring = NULL;
  native_luaL_optlstring = NULL;
  native_luaL_checknumber = NULL;
  native_luaL_optnumber = NULL;
  native_luaL_checkinteger = NULL;
  native_luaL_optinteger = NULL;
  native_luaL_checkstack = NULL;
  native_luaL_checktype = NULL;
  native_luaL_checkany = NULL;
  native_luaL_newmetatable = NULL;
  native_luaL_checkudata = NULL;
  native_luaL_where = NULL;
  native_luaL_error = NULL;
  native_luaL_checkoption = NULL;
  native_luaL_ref = NULL;
  native_luaL_unref = NULL;
  native_luaL_loadfile = NULL;
  native_luaL_loadbuffer = NULL;
  native_luaL_loadstring = NULL;
  native_luaL_gsub = NULL;
  native_luaL_findtable = NULL;
  native_luaL_buffinit = NULL;
  native_luaL_prepbuffer = NULL;
  native_luaL_addlstring = NULL;
  native_luaL_addstring = NULL;
  native_luaL_addvalue = NULL;
  native_luaL_pushresult = NULL;
  native_luaopen_base = NULL;
  native_luaopen_table = NULL;
  native_luaopen_io = NULL;
  native_luaopen_os = NULL;
  native_luaopen_string = NULL;
  native_luaopen_math = NULL;
  native_luaopen_debug = NULL;
  native_luaopen_package = NULL;
  native_luaL_openlibs = NULL;

}

 LUA_API lua_State *LAZYAPICALL lua_newstate (lua_Alloc f, void *ud){
    if (!native_lua_newstate)
        load_symbol_or_err("lua_newstate", (gpointer *) &native_lua_newstate);
    return native_lua_newstate(f, ud);
}
 LUA_API void LAZYAPICALL lua_close (lua_State *L){
    if (!native_lua_close)
        load_symbol_or_err("lua_close", (gpointer *) &native_lua_close);
    /*return*/ native_lua_close(L);
}
 LUA_API lua_State *LAZYAPICALL lua_newthread (lua_State *L){
    if (!native_lua_newthread)
        load_symbol_or_err("lua_newthread", (gpointer *) &native_lua_newthread);
    return native_lua_newthread(L);
}
 LUA_API LAZYAPICALL lua_CFunction (lua_atpanic) (lua_State *L, lua_CFunction panicf){
    if (!native_lua_atpanic)
        load_symbol_or_err("lua_atpanic", (gpointer *) &native_lua_atpanic);
    return native_lua_atpanic(L, panicf);
}
 LUA_API int LAZYAPICALL lua_gettop (lua_State *L){
    if (!native_lua_gettop)
        load_symbol_or_err("lua_gettop", (gpointer *) &native_lua_gettop);
    return native_lua_gettop(L);
}
 LUA_API void LAZYAPICALL lua_settop (lua_State *L, int idx){
    if (!native_lua_settop)
        load_symbol_or_err("lua_settop", (gpointer *) &native_lua_settop);
    /*return*/ native_lua_settop(L, idx);
}
 LUA_API void LAZYAPICALL lua_pushvalue (lua_State *L, int idx){
    if (!native_lua_pushvalue)
        load_symbol_or_err("lua_pushvalue", (gpointer *) &native_lua_pushvalue);
    /*return*/ native_lua_pushvalue(L, idx);
}
 LUA_API void LAZYAPICALL lua_remove (lua_State *L, int idx){
    if (!native_lua_remove)
        load_symbol_or_err("lua_remove", (gpointer *) &native_lua_remove);
    /*return*/ native_lua_remove(L, idx);
}
 LUA_API void LAZYAPICALL lua_insert (lua_State *L, int idx){
    if (!native_lua_insert)
        load_symbol_or_err("lua_insert", (gpointer *) &native_lua_insert);
    /*return*/ native_lua_insert(L, idx);
}
 LUA_API void LAZYAPICALL lua_replace (lua_State *L, int idx){
    if (!native_lua_replace)
        load_symbol_or_err("lua_replace", (gpointer *) &native_lua_replace);
    /*return*/ native_lua_replace(L, idx);
}
 LUA_API int LAZYAPICALL lua_checkstack (lua_State *L, int sz){
    if (!native_lua_checkstack)
        load_symbol_or_err("lua_checkstack", (gpointer *) &native_lua_checkstack);
    return native_lua_checkstack(L, sz);
}
 LUA_API void LAZYAPICALL lua_xmove (lua_State *from, lua_State *to, int n){
    if (!native_lua_xmove)
        load_symbol_or_err("lua_xmove", (gpointer *) &native_lua_xmove);
    /*return*/ native_lua_xmove(from, to, n);
}
 LUA_API int LAZYAPICALL lua_isnumber (lua_State *L, int idx){
    if (!native_lua_isnumber)
        load_symbol_or_err("lua_isnumber", (gpointer *) &native_lua_isnumber);
    return native_lua_isnumber(L, idx);
}
 LUA_API int LAZYAPICALL lua_isstring (lua_State *L, int idx){
    if (!native_lua_isstring)
        load_symbol_or_err("lua_isstring", (gpointer *) &native_lua_isstring);
    return native_lua_isstring(L, idx);
}
 LUA_API int LAZYAPICALL lua_iscfunction (lua_State *L, int idx){
    if (!native_lua_iscfunction)
        load_symbol_or_err("lua_iscfunction", (gpointer *) &native_lua_iscfunction);
    return native_lua_iscfunction(L, idx);
}
 LUA_API int LAZYAPICALL lua_isuserdata (lua_State *L, int idx){
    if (!native_lua_isuserdata)
        load_symbol_or_err("lua_isuserdata", (gpointer *) &native_lua_isuserdata);
    return native_lua_isuserdata(L, idx);
}
 LUA_API int LAZYAPICALL lua_type (lua_State *L, int idx){
    if (!native_lua_type)
        load_symbol_or_err("lua_type", (gpointer *) &native_lua_type);
    return native_lua_type(L, idx);
}
 LUA_API const char *LAZYAPICALL lua_typename (lua_State *L, int tp){
    if (!native_lua_typename)
        load_symbol_or_err("lua_typename", (gpointer *) &native_lua_typename);
    return native_lua_typename(L, tp);
}
 LUA_API int LAZYAPICALL lua_equal (lua_State *L, int idx1, int idx2){
    if (!native_lua_equal)
        load_symbol_or_err("lua_equal", (gpointer *) &native_lua_equal);
    return native_lua_equal(L, idx1, idx2);
}
 LUA_API int LAZYAPICALL lua_rawequal (lua_State *L, int idx1, int idx2){
    if (!native_lua_rawequal)
        load_symbol_or_err("lua_rawequal", (gpointer *) &native_lua_rawequal);
    return native_lua_rawequal(L, idx1, idx2);
}
 LUA_API int LAZYAPICALL lua_lessthan (lua_State *L, int idx1, int idx2){
    if (!native_lua_lessthan)
        load_symbol_or_err("lua_lessthan", (gpointer *) &native_lua_lessthan);
    return native_lua_lessthan(L, idx1, idx2);
}
 LUA_API lua_Number LAZYAPICALL lua_tonumber (lua_State *L, int idx){
    if (!native_lua_tonumber)
        load_symbol_or_err("lua_tonumber", (gpointer *) &native_lua_tonumber);
    return native_lua_tonumber(L, idx);
}
 LUA_API lua_Integer LAZYAPICALL lua_tointeger (lua_State *L, int idx){
    if (!native_lua_tointeger)
        load_symbol_or_err("lua_tointeger", (gpointer *) &native_lua_tointeger);
    return native_lua_tointeger(L, idx);
}
 LUA_API int LAZYAPICALL lua_toboolean (lua_State *L, int idx){
    if (!native_lua_toboolean)
        load_symbol_or_err("lua_toboolean", (gpointer *) &native_lua_toboolean);
    return native_lua_toboolean(L, idx);
}
 LUA_API const char *LAZYAPICALL lua_tolstring (lua_State *L, int idx, size_t *len){
    if (!native_lua_tolstring)
        load_symbol_or_err("lua_tolstring", (gpointer *) &native_lua_tolstring);
    return native_lua_tolstring(L, idx, len);
}
 LUA_API size_t LAZYAPICALL lua_objlen (lua_State *L, int idx){
    if (!native_lua_objlen)
        load_symbol_or_err("lua_objlen", (gpointer *) &native_lua_objlen);
    return native_lua_objlen(L, idx);
}
 LUA_API lua_CFunction LAZYAPICALL lua_tocfunction (lua_State *L, int idx){
    if (!native_lua_tocfunction)
        load_symbol_or_err("lua_tocfunction", (gpointer *) &native_lua_tocfunction);
    return native_lua_tocfunction(L, idx);
}
 LUA_API void *LAZYAPICALL lua_touserdata (lua_State *L, int idx){
    if (!native_lua_touserdata)
        load_symbol_or_err("lua_touserdata", (gpointer *) &native_lua_touserdata);
    return native_lua_touserdata(L, idx);
}
 LUA_API lua_State *LAZYAPICALL lua_tothread (lua_State *L, int idx){
    if (!native_lua_tothread)
        load_symbol_or_err("lua_tothread", (gpointer *) &native_lua_tothread);
    return native_lua_tothread(L, idx);
}
 LUA_API const void *LAZYAPICALL lua_topointer (lua_State *L, int idx){
    if (!native_lua_topointer)
        load_symbol_or_err("lua_topointer", (gpointer *) &native_lua_topointer);
    return native_lua_topointer(L, idx);
}
 LUA_API void LAZYAPICALL lua_pushnil (lua_State *L){
    if (!native_lua_pushnil)
        load_symbol_or_err("lua_pushnil", (gpointer *) &native_lua_pushnil);
    /*return*/ native_lua_pushnil(L);
}
 LUA_API void LAZYAPICALL lua_pushnumber (lua_State *L, lua_Number n){
    if (!native_lua_pushnumber)
        load_symbol_or_err("lua_pushnumber", (gpointer *) &native_lua_pushnumber);
    /*return*/ native_lua_pushnumber(L, n);
}
 LUA_API void LAZYAPICALL lua_pushinteger (lua_State *L, lua_Integer n){
    if (!native_lua_pushinteger)
        load_symbol_or_err("lua_pushinteger", (gpointer *) &native_lua_pushinteger);
    /*return*/ native_lua_pushinteger(L, n);
}
 LUA_API void LAZYAPICALL lua_pushlstring (lua_State *L, const char *s, size_t l){
    if (!native_lua_pushlstring)
        load_symbol_or_err("lua_pushlstring", (gpointer *) &native_lua_pushlstring);
    /*return*/ native_lua_pushlstring(L, s, l);
}
 LUA_API void LAZYAPICALL lua_pushstring (lua_State *L, const char *s){
    if (!native_lua_pushstring)
        load_symbol_or_err("lua_pushstring", (gpointer *) &native_lua_pushstring);
    /*return*/ native_lua_pushstring(L, s);
}
 LUA_API const char *LAZYAPICALL lua_pushvfstring (lua_State *L, const char *fmt, va_list argp){
    if (!native_lua_pushvfstring)
        load_symbol_or_err("lua_pushvfstring", (gpointer *) &native_lua_pushvfstring);
    return native_lua_pushvfstring(L, fmt, argp);
}
 LUA_API const char *LAZYAPICALL lua_pushfstring (lua_State *L, const char *fmt, ...){
    if (!native_lua_pushfstring)
        load_symbol_or_err("lua_pushfstring", (gpointer *) &native_lua_pushfstring);
    return native_lua_pushfstring(L, fmt);
}
 LUA_API void LAZYAPICALL lua_pushcclosure (lua_State *L, lua_CFunction fn, int n){
    if (!native_lua_pushcclosure)
        load_symbol_or_err("lua_pushcclosure", (gpointer *) &native_lua_pushcclosure);
    /*return*/ native_lua_pushcclosure(L, fn, n);
}
 LUA_API void LAZYAPICALL lua_pushboolean (lua_State *L, int b){
    if (!native_lua_pushboolean)
        load_symbol_or_err("lua_pushboolean", (gpointer *) &native_lua_pushboolean);
    /*return*/ native_lua_pushboolean(L, b);
}
 LUA_API void LAZYAPICALL lua_pushlightuserdata (lua_State *L, void *p){
    if (!native_lua_pushlightuserdata)
        load_symbol_or_err("lua_pushlightuserdata", (gpointer *) &native_lua_pushlightuserdata);
    /*return*/ native_lua_pushlightuserdata(L, p);
}
 LUA_API int LAZYAPICALL lua_pushthread (lua_State *L){
    if (!native_lua_pushthread)
        load_symbol_or_err("lua_pushthread", (gpointer *) &native_lua_pushthread);
    return native_lua_pushthread(L);
}
 LUA_API void LAZYAPICALL lua_gettable (lua_State *L, int idx){
    if (!native_lua_gettable)
        load_symbol_or_err("lua_gettable", (gpointer *) &native_lua_gettable);
    /*return*/ native_lua_gettable(L, idx);
}
 LUA_API void LAZYAPICALL lua_getfield (lua_State *L, int idx, const char *k){
    if (!native_lua_getfield)
        load_symbol_or_err("lua_getfield", (gpointer *) &native_lua_getfield);
    /*return*/ native_lua_getfield(L, idx, k);
}
 LUA_API void LAZYAPICALL lua_rawget (lua_State *L, int idx){
    if (!native_lua_rawget)
        load_symbol_or_err("lua_rawget", (gpointer *) &native_lua_rawget);
    /*return*/ native_lua_rawget(L, idx);
}
 LUA_API void LAZYAPICALL lua_rawgeti (lua_State *L, int idx, int n){
    if (!native_lua_rawgeti)
        load_symbol_or_err("lua_rawgeti", (gpointer *) &native_lua_rawgeti);
    /*return*/ native_lua_rawgeti(L, idx, n);
}
 LUA_API void LAZYAPICALL lua_createtable (lua_State *L, int narr, int nrec){
    if (!native_lua_createtable)
        load_symbol_or_err("lua_createtable", (gpointer *) &native_lua_createtable);
    /*return*/ native_lua_createtable(L, narr, nrec);
}
 LUA_API void *LAZYAPICALL lua_newuserdata (lua_State *L, size_t sz){
    if (!native_lua_newuserdata)
        load_symbol_or_err("lua_newuserdata", (gpointer *) &native_lua_newuserdata);
    return native_lua_newuserdata(L, sz);
}
 LUA_API int LAZYAPICALL lua_getmetatable (lua_State *L, int objindex){
    if (!native_lua_getmetatable)
        load_symbol_or_err("lua_getmetatable", (gpointer *) &native_lua_getmetatable);
    return native_lua_getmetatable(L, objindex);
}
 LUA_API void LAZYAPICALL lua_getfenv (lua_State *L, int idx){
    if (!native_lua_getfenv)
        load_symbol_or_err("lua_getfenv", (gpointer *) &native_lua_getfenv);
    /*return*/ native_lua_getfenv(L, idx);
}
 LUA_API void LAZYAPICALL lua_settable (lua_State *L, int idx){
    if (!native_lua_settable)
        load_symbol_or_err("lua_settable", (gpointer *) &native_lua_settable);
    /*return*/ native_lua_settable(L, idx);
}
 LUA_API void LAZYAPICALL lua_setfield (lua_State *L, int idx, const char *k){
    if (!native_lua_setfield)
        load_symbol_or_err("lua_setfield", (gpointer *) &native_lua_setfield);
    /*return*/ native_lua_setfield(L, idx, k);
}
 LUA_API void LAZYAPICALL lua_rawset (lua_State *L, int idx){
    if (!native_lua_rawset)
        load_symbol_or_err("lua_rawset", (gpointer *) &native_lua_rawset);
    /*return*/ native_lua_rawset(L, idx);
}
 LUA_API void LAZYAPICALL lua_rawseti (lua_State *L, int idx, int n){
    if (!native_lua_rawseti)
        load_symbol_or_err("lua_rawseti", (gpointer *) &native_lua_rawseti);
    /*return*/ native_lua_rawseti(L, idx, n);
}
 LUA_API int LAZYAPICALL lua_setmetatable (lua_State *L, int objindex){
    if (!native_lua_setmetatable)
        load_symbol_or_err("lua_setmetatable", (gpointer *) &native_lua_setmetatable);
    return native_lua_setmetatable(L, objindex);
}
 LUA_API int LAZYAPICALL lua_setfenv (lua_State *L, int idx){
    if (!native_lua_setfenv)
        load_symbol_or_err("lua_setfenv", (gpointer *) &native_lua_setfenv);
    return native_lua_setfenv(L, idx);
}
 LUA_API void LAZYAPICALL lua_call (lua_State *L, int nargs, int nresults){
    if (!native_lua_call)
        load_symbol_or_err("lua_call", (gpointer *) &native_lua_call);
    /*return*/ native_lua_call(L, nargs, nresults);
}
 LUA_API int LAZYAPICALL lua_pcall (lua_State *L, int nargs, int nresults, int errfunc){
    if (!native_lua_pcall)
        load_symbol_or_err("lua_pcall", (gpointer *) &native_lua_pcall);
    return native_lua_pcall(L, nargs, nresults, errfunc);
}
 LUA_API int LAZYAPICALL lua_cpcall (lua_State *L, lua_CFunction func, void *ud){
    if (!native_lua_cpcall)
        load_symbol_or_err("lua_cpcall", (gpointer *) &native_lua_cpcall);
    return native_lua_cpcall(L, func, ud);
}
 LUA_API int LAZYAPICALL lua_load (lua_State *L, lua_Reader reader, void *dt, const char *chunkname){
    if (!native_lua_load)
        load_symbol_or_err("lua_load", (gpointer *) &native_lua_load);
    return native_lua_load(L, reader, dt, chunkname);
}
 LUA_API int LAZYAPICALL lua_dump (lua_State *L, lua_Writer writer, void *data){
    if (!native_lua_dump)
        load_symbol_or_err("lua_dump", (gpointer *) &native_lua_dump);
    return native_lua_dump(L, writer, data);
}
 LUA_API int LAZYAPICALL lua_yield (lua_State *L, int nresults){
    if (!native_lua_yield)
        load_symbol_or_err("lua_yield", (gpointer *) &native_lua_yield);
    return native_lua_yield(L, nresults);
}
 LUA_API int LAZYAPICALL lua_resume (lua_State *L, int narg){
    if (!native_lua_resume)
        load_symbol_or_err("lua_resume", (gpointer *) &native_lua_resume);
    return native_lua_resume(L, narg);
}
 LUA_API int LAZYAPICALL lua_status (lua_State *L){
    if (!native_lua_status)
        load_symbol_or_err("lua_status", (gpointer *) &native_lua_status);
    return native_lua_status(L);
}
 LUA_API int LAZYAPICALL lua_gc (lua_State *L, int what, int data){
    if (!native_lua_gc)
        load_symbol_or_err("lua_gc", (gpointer *) &native_lua_gc);
    return native_lua_gc(L, what, data);
}
 LUA_API int LAZYAPICALL lua_error (lua_State *L){
    if (!native_lua_error)
        load_symbol_or_err("lua_error", (gpointer *) &native_lua_error);
    return native_lua_error(L);
}
 LUA_API int LAZYAPICALL lua_next (lua_State *L, int idx){
    if (!native_lua_next)
        load_symbol_or_err("lua_next", (gpointer *) &native_lua_next);
    return native_lua_next(L, idx);
}
 LUA_API void LAZYAPICALL lua_concat (lua_State *L, int n){
    if (!native_lua_concat)
        load_symbol_or_err("lua_concat", (gpointer *) &native_lua_concat);
    /*return*/ native_lua_concat(L, n);
}
 LUA_API lua_Alloc LAZYAPICALL lua_getallocf (lua_State *L, void **ud){
    if (!native_lua_getallocf)
        load_symbol_or_err("lua_getallocf", (gpointer *) &native_lua_getallocf);
    return native_lua_getallocf(L, ud);
}
 LUA_API void LAZYAPICALL lua_setallocf (lua_State *L, lua_Alloc f, void *ud){
    if (!native_lua_setallocf)
        load_symbol_or_err("lua_setallocf", (gpointer *) &native_lua_setallocf);
    /*return*/ native_lua_setallocf(L, f, ud);
}
 LUA_API void LAZYAPICALL lua_setlevel (lua_State *from, lua_State *to){
    if (!native_lua_setlevel)
        load_symbol_or_err("lua_setlevel", (gpointer *) &native_lua_setlevel);
    /*return*/ native_lua_setlevel(from, to);
}
 LUA_API int LAZYAPICALL lua_getstack (lua_State *L, int level, lua_Debug *ar){
    if (!native_lua_getstack)
        load_symbol_or_err("lua_getstack", (gpointer *) &native_lua_getstack);
    return native_lua_getstack(L, level, ar);
}
 LUA_API int LAZYAPICALL lua_getinfo (lua_State *L, const char *what, lua_Debug *ar){
    if (!native_lua_getinfo)
        load_symbol_or_err("lua_getinfo", (gpointer *) &native_lua_getinfo);
    return native_lua_getinfo(L, what, ar);
}
 LUA_API const char *LAZYAPICALL lua_getlocal (lua_State *L, const lua_Debug *ar, int n){
    if (!native_lua_getlocal)
        load_symbol_or_err("lua_getlocal", (gpointer *) &native_lua_getlocal);
    return native_lua_getlocal(L, ar, n);
}
 LUA_API const char *LAZYAPICALL lua_setlocal (lua_State *L, const lua_Debug *ar, int n){
    if (!native_lua_setlocal)
        load_symbol_or_err("lua_setlocal", (gpointer *) &native_lua_setlocal);
    return native_lua_setlocal(L, ar, n);
}
 LUA_API const char *LAZYAPICALL lua_getupvalue (lua_State *L, int funcindex, int n){
    if (!native_lua_getupvalue)
        load_symbol_or_err("lua_getupvalue", (gpointer *) &native_lua_getupvalue);
    return native_lua_getupvalue(L, funcindex, n);
}
 LUA_API const char *LAZYAPICALL lua_setupvalue (lua_State *L, int funcindex, int n){
    if (!native_lua_setupvalue)
        load_symbol_or_err("lua_setupvalue", (gpointer *) &native_lua_setupvalue);
    return native_lua_setupvalue(L, funcindex, n);
}
 LUA_API int LAZYAPICALL lua_sethook (lua_State *L, lua_Hook func, int mask, int count){
    if (!native_lua_sethook)
        load_symbol_or_err("lua_sethook", (gpointer *) &native_lua_sethook);
    return native_lua_sethook(L, func, mask, count);
}
 LUA_API lua_Hook LAZYAPICALL lua_gethook (lua_State *L){
    if (!native_lua_gethook)
        load_symbol_or_err("lua_gethook", (gpointer *) &native_lua_gethook);
    return native_lua_gethook(L);
}
 LUA_API int LAZYAPICALL lua_gethookmask (lua_State *L){
    if (!native_lua_gethookmask)
        load_symbol_or_err("lua_gethookmask", (gpointer *) &native_lua_gethookmask);
    return native_lua_gethookmask(L);
}
 LUA_API int LAZYAPICALL lua_gethookcount (lua_State *L){
    if (!native_lua_gethookcount)
        load_symbol_or_err("lua_gethookcount", (gpointer *) &native_lua_gethookcount);
    return native_lua_gethookcount(L);
}

 // lauxlib.h
 LUALIB_API lua_State * LAZYAPICALL luaL_newstate(void) {
   if (!native_luaL_newstate)
     load_symbol_or_err("luaL_newstate", (gpointer *) &native_luaL_newstate);
  return native_luaL_newstate();
}

 LUALIB_API void LAZYAPICALL luaI_openlib (lua_State *L, const char *libname, const luaL_Reg *l, int nup){
    if (!native_luaI_openlib)
        load_symbol_or_err("luaI_openlib", (gpointer *) &native_luaI_openlib);
    /*return*/ native_luaI_openlib(L, libname, l, nup);
}
 LUALIB_API void LAZYAPICALL luaL_register (lua_State *L, const char *libname, const luaL_Reg *l){
    if (!native_luaL_register)
        load_symbol_or_err("luaL_register", (gpointer *) &native_luaL_register);
    /*return*/ native_luaL_register(L, libname, l);
}
 LUALIB_API int LAZYAPICALL luaL_getmetafield (lua_State *L, int obj, const char *e){
    if (!native_luaL_getmetafield)
        load_symbol_or_err("luaL_getmetafield", (gpointer *) &native_luaL_getmetafield);
    return native_luaL_getmetafield(L, obj, e);
}
 LUALIB_API int LAZYAPICALL luaL_callmeta (lua_State *L, int obj, const char *e){
    if (!native_luaL_callmeta)
        load_symbol_or_err("luaL_callmeta", (gpointer *) &native_luaL_callmeta);
    return native_luaL_callmeta(L, obj, e);
}
 LUALIB_API int LAZYAPICALL luaL_typerror (lua_State *L, int narg, const char *tname){
    if (!native_luaL_typerror)
        load_symbol_or_err("luaL_typerror", (gpointer *) &native_luaL_typerror);
    return native_luaL_typerror(L, narg, tname);
}
 LUALIB_API int LAZYAPICALL luaL_argerror (lua_State *L, int numarg, const char *extramsg){
    if (!native_luaL_argerror)
        load_symbol_or_err("luaL_argerror", (gpointer *) &native_luaL_argerror);
    return native_luaL_argerror(L, numarg, extramsg);
}
 LUALIB_API const char *LAZYAPICALL luaL_checklstring (lua_State *L, int numArg, size_t *l){
    if (!native_luaL_checklstring)
        load_symbol_or_err("luaL_checklstring", (gpointer *) &native_luaL_checklstring);
    return native_luaL_checklstring(L, numArg, l);
}
 LUALIB_API const char *LAZYAPICALL luaL_optlstring (lua_State *L, int numArg, const char *def, size_t *l){
    if (!native_luaL_optlstring)
        load_symbol_or_err("luaL_optlstring", (gpointer *) &native_luaL_optlstring);
    return native_luaL_optlstring(L, numArg, def, l);
}
 LUALIB_API lua_Number LAZYAPICALL luaL_checknumber (lua_State *L, int numArg){
    if (!native_luaL_checknumber)
        load_symbol_or_err("luaL_checknumber", (gpointer *) &native_luaL_checknumber);
    return native_luaL_checknumber(L, numArg);
}
 LUALIB_API lua_Number LAZYAPICALL luaL_optnumber (lua_State *L, int nArg, lua_Number def){
    if (!native_luaL_optnumber)
        load_symbol_or_err("luaL_optnumber", (gpointer *) &native_luaL_optnumber);
    return native_luaL_optnumber(L, nArg, def);
}
 LUALIB_API lua_Integer LAZYAPICALL luaL_checkinteger (lua_State *L, int numArg){
    if (!native_luaL_checkinteger)
        load_symbol_or_err("luaL_checkinteger", (gpointer *) &native_luaL_checkinteger);
    return native_luaL_checkinteger(L, numArg);
}
 LUALIB_API lua_Integer LAZYAPICALL luaL_optinteger (lua_State *L, int nArg, lua_Integer def){
    if (!native_luaL_optinteger)
        load_symbol_or_err("luaL_optinteger", (gpointer *) &native_luaL_optinteger);
    return native_luaL_optinteger(L, nArg, def);
}
 LUALIB_API void LAZYAPICALL luaL_checkstack (lua_State *L, int sz, const char *msg){
    if (!native_luaL_checkstack)
        load_symbol_or_err("luaL_checkstack", (gpointer *) &native_luaL_checkstack);
    /*return*/ native_luaL_checkstack(L, sz, msg);
}
 LUALIB_API void LAZYAPICALL luaL_checktype (lua_State *L, int narg, int t){
    if (!native_luaL_checktype)
        load_symbol_or_err("luaL_checktype", (gpointer *) &native_luaL_checktype);
    /*return*/ native_luaL_checktype(L, narg, t);
}
 LUALIB_API void LAZYAPICALL luaL_checkany (lua_State *L, int narg){
    if (!native_luaL_checkany)
        load_symbol_or_err("luaL_checkany", (gpointer *) &native_luaL_checkany);
    /*return*/ native_luaL_checkany(L, narg);
}
 LUALIB_API int LAZYAPICALL luaL_newmetatable (lua_State *L, const char *tname){
    if (!native_luaL_newmetatable)
        load_symbol_or_err("luaL_newmetatable", (gpointer *) &native_luaL_newmetatable);
    return native_luaL_newmetatable(L, tname);
}
 LUALIB_API void *LAZYAPICALL luaL_checkudata (lua_State *L, int ud, const char *tname){
    if (!native_luaL_checkudata)
        load_symbol_or_err("luaL_checkudata", (gpointer *) &native_luaL_checkudata);
    return native_luaL_checkudata(L, ud, tname);
}
 LUALIB_API void LAZYAPICALL luaL_where (lua_State *L, int lvl){
    if (!native_luaL_where)
        load_symbol_or_err("luaL_where", (gpointer *) &native_luaL_where);
    /*return*/ native_luaL_where(L, lvl);
}
 LUALIB_API int LAZYAPICALL luaL_error (lua_State *L, const char *fmt, ...){
    if (!native_luaL_error)
        load_symbol_or_err("luaL_error", (gpointer *) &native_luaL_error);
    return native_luaL_error(L, fmt);
}
 LUALIB_API int LAZYAPICALL luaL_checkoption (lua_State *L, int narg, const char *def, const char *const lst[]){
    if (!native_luaL_checkoption)
        load_symbol_or_err("luaL_checkoption", (gpointer *) &native_luaL_checkoption);
    return native_luaL_checkoption(L, narg, def, lst);
}
 LUALIB_API int LAZYAPICALL luaL_ref (lua_State *L, int t){
    if (!native_luaL_ref)
        load_symbol_or_err("luaL_ref", (gpointer *) &native_luaL_ref);
    return native_luaL_ref(L, t);
}
 LUALIB_API void LAZYAPICALL luaL_unref (lua_State *L, int t, int ref){
    if (!native_luaL_unref)
        load_symbol_or_err("luaL_unref", (gpointer *) &native_luaL_unref);
    /*return*/ native_luaL_unref(L, t, ref);
}
 LUALIB_API int LAZYAPICALL luaL_loadfile (lua_State *L, const char *filename){
    if (!native_luaL_loadfile)
        load_symbol_or_err("luaL_loadfile", (gpointer *) &native_luaL_loadfile);
    return native_luaL_loadfile(L, filename);
}
 LUALIB_API int LAZYAPICALL luaL_loadbuffer (lua_State *L, const char *buff, size_t sz, const char *name){
    if (!native_luaL_loadbuffer)
        load_symbol_or_err("luaL_loadbuffer", (gpointer *) &native_luaL_loadbuffer);
    return native_luaL_loadbuffer(L, buff, sz, name);
}
 LUALIB_API int LAZYAPICALL luaL_loadstring (lua_State *L, const char *s){
    if (!native_luaL_loadstring)
        load_symbol_or_err("luaL_loadstring", (gpointer *) &native_luaL_loadstring);
    return native_luaL_loadstring(L, s);
}

 LUALIB_API const char *LAZYAPICALL luaL_gsub (lua_State *L, const char *s, const char *p, const char *r){
    if (!native_luaL_gsub)
        load_symbol_or_err("luaL_gsub", (gpointer *) &native_luaL_gsub);
    return native_luaL_gsub(L, s, p, r);
}
 LUALIB_API const char *LAZYAPICALL luaL_findtable (lua_State *L, int idx, const char *fname, int szhint){
    if (!native_luaL_findtable)
        load_symbol_or_err("luaL_findtable", (gpointer *) &native_luaL_findtable);
    return native_luaL_findtable(L, idx, fname, szhint);
}
 LUALIB_API void LAZYAPICALL luaL_buffinit (lua_State *L, luaL_Buffer *B){
    if (!native_luaL_buffinit)
        load_symbol_or_err("luaL_buffinit", (gpointer *) &native_luaL_buffinit);
    /*return*/ native_luaL_buffinit(L, B);
}
 LUALIB_API char *LAZYAPICALL luaL_prepbuffer (luaL_Buffer *B){
    if (!native_luaL_prepbuffer)
        load_symbol_or_err("luaL_prepbuffer", (gpointer *) &native_luaL_prepbuffer);
    return native_luaL_prepbuffer(B);
}
 LUALIB_API void LAZYAPICALL luaL_addlstring (luaL_Buffer *B, const char *s, size_t l){
    if (!native_luaL_addlstring)
        load_symbol_or_err("luaL_addlstring", (gpointer *) &native_luaL_addlstring);
    /*return*/ native_luaL_addlstring(B, s, l);
}
 LUALIB_API void LAZYAPICALL luaL_addstring (luaL_Buffer *B, const char *s){
    if (!native_luaL_addstring)
        load_symbol_or_err("luaL_addstring", (gpointer *) &native_luaL_addstring);
    /*return*/ native_luaL_addstring(B, s);
}
 LUALIB_API void LAZYAPICALL luaL_addvalue (luaL_Buffer *B){
    if (!native_luaL_addvalue)
        load_symbol_or_err("luaL_addvalue", (gpointer *) &native_luaL_addvalue);
    /*return*/ native_luaL_addvalue(B);
}
 LUALIB_API void LAZYAPICALL luaL_pushresult (luaL_Buffer *B){
    if (!native_luaL_pushresult)
        load_symbol_or_err("luaL_pushresult", (gpointer *) &native_luaL_pushresult);
    /*return*/ native_luaL_pushresult(B);
}

//lualib.h
 LUALIB_API int LAZYAPICALL luaopen_base (lua_State *L){
   if (!native_luaopen_base)
     load_symbol_or_err("luaopen_base", (gpointer *) &native_luaopen_base);
   return native_luaopen_base(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_table (lua_State *L){
   if (!native_luaopen_table)
     load_symbol_or_err("luaopen_table", (gpointer *) &native_luaopen_table);
   return native_luaopen_table(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_io (lua_State *L){
   if (!native_luaopen_io)
     load_symbol_or_err("luaopen_io", (gpointer *) &native_luaopen_io);
   return native_luaopen_io(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_os (lua_State *L){
   if (!native_luaopen_os)
     load_symbol_or_err("luaopen_os", (gpointer *) &native_luaopen_os);
   return native_luaopen_os(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_string (lua_State *L){
   if (!native_luaopen_string)
     load_symbol_or_err("luaopen_string", (gpointer *) &native_luaopen_string);
   return native_luaopen_string(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_math (lua_State *L){
   if (!native_luaopen_math)
     load_symbol_or_err("luaopen_math", (gpointer *) &native_luaopen_math);
   return native_luaopen_math(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_debug (lua_State *L){
   if (!native_luaopen_debug)
     load_symbol_or_err("luaopen_debug", (gpointer *) &native_luaopen_debug);
   return native_luaopen_debug(L);
 }
 LUALIB_API int LAZYAPICALL luaopen_package (lua_State *L){
   if (!native_luaopen_package)
     load_symbol_or_err("luaopen_package", (gpointer *) &native_luaopen_package);
   return native_luaopen_package(L);
 }
 LUALIB_API void LAZYAPICALL luaL_openlibs (lua_State *L){
   if (!native_luaL_openlibs)
     load_symbol_or_err("luaL_openlibs", (gpointer *) &native_luaL_openlibs);
   /*return*/ native_luaL_openlibs(L);
 }