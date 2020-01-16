local CMAKE_CFG_INTDIR = ... or '.'
local modules = {
  ['strict'] = '/home/mka/opt/xscript/lua/luadist/5.1.5/etc/strict.lua';
}
local function preload_modules(modules)
  for name, path in pairs(modules) do
    if path:match'%.lua' then
      package.preload[name] = assert(loadfile(path))
    else
      local name = name:gsub('.*%-', '') -- remove any hyphen prefix
      local symbol = 'luaopen_' .. name:gsub('%.', '_')
          --improve: generalize to support all-in-one loader?
      local path = path:gsub('%$%{CMAKE_CFG_INTDIR%}', CMAKE_CFG_INTDIR)
      package.preload[name] = assert(package.loadlib(path, symbol))
    end
  end
end
preload_modules(modules)
arg[0] = '/home/mka/opt/xscript/lua/luadist/5.1.5/test/trace-globals.lua'
table.remove(arg, 1)
return assert(loadfile '/home/mka/opt/xscript/lua/luadist/5.1.5/test/trace-globals.lua')(unpack(arg))
