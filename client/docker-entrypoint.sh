#!/bin/sh
# Simple entrypoint: print diagnostics useful for debugging Qt/X11 issues,
# then exec the provided command (default: ./main)

echo "ENTRYPOINT: host -> $(hostname)"
echo "DISPLAY=${DISPLAY:-not-set}"
# show which libraries the Qt xcb plugin would load if present
if [ -d "/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms" ]; then
  echo "Qt platforms plugin directory contents:"
  ls -l /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms || true
fi

# show ldd of potential xcb plugin if present (helps identify missing deps)
if [ -f "/usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/libqxcb.so" ]; then
  echo "ldd libqxcb.so ->"
  ldd /usr/lib/x86_64-linux-gnu/qt5/plugins/platforms/libqxcb.so || true
fi

# print environment
echo "Environment variables relevant to Qt/X11:"
env | egrep 'DISPLAY|QT_|XDG|LD_LIBRARY_PATH' || true

# exec provided cmd
exec "$@"
