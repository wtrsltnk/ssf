
CPMAddPackage(
    NAME system.io
    GITHUB_REPOSITORY wtrsltnk/system.io
    GIT_TAG v1.0.0
)

# HACK HACK
include_directories(${system.io_SOURCE_DIR}/include)

CPMAddPackage(
    NAME system.net
    GITHUB_REPOSITORY wtrsltnk/system.net
    GIT_TAG v1.0.4
)

# HACK HACK
include_directories(${system.net_SOURCE_DIR}/include)
