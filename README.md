# Serve Static Files (ssf)

When ssf.exe is run without arguments, it will serve static files from the current working directory on the port 8888. Open http://localhost:8888/ to visit the served files.

```
usage:
  ssf.exe [<path>] options

where arguments are:
  <path>               which path to serve static files from? Default is current working directory.

where options are:
  -p, --port <port>    on what should I be listing? Default is 8888
  -?, -h, --help       display usage information
```

## TODO

- ~~Make sure the correct ``Content-Type`` header is rerturned~~
- ~~Make sure binary files are supported~~
- Add directory browsing
