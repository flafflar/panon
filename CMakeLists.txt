cmake_minimum_required(VERSION 3.16)

project(panon)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TODO: Set this per target?
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

include(cmake/plasmoid.cmake)

set(QT_MIN_VERSION "6.0.0")
set(KF6_MIN_VERSION "6.0.0")

#find_package(ECM ${KF6_MIN_VERSION} REQUIRED NO_MODULE)

include(KDECMakeSettings)
include(KDECompilerSettings NO_POLICY_SCOPE)
include(FeatureSummary)

find_package(Qt6 ${QT_MIN_VERSION} CONFIG REQUIRED
  COMPONENTS
    Core
    Quick
    Widgets
    Multimedia
    ShaderTools
)

find_package(KF6 ${KF6_MIN_VERSION} REQUIRED COMPONENTS
    I18n
    Config
)

find_package(Plasma REQUIRED)

add_subdirectory(src)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
