set history save
set history filename ~/.gdb_history

define reconnect
    target remote :3333
end

define reload
    mon reset halt
    make
    load
    mon reset init
    continue
end
