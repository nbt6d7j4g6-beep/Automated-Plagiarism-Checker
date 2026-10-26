
cmake_minimum_required(VERSION 3.21)

project(PlagiarismCheckerGUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

qt_standard_project_setup()

qt_add_executable(PlagiarismCheckerGUI
    main.cpp
    ../src/FileManager.cpp
    ../src/TextProcessor.cpp
    ../src/NGramGenerator.cpp
    ../src/SimilarityCalculator.cpp
    ../src/ComparisonManager.cpp
    ../src/PlagiarismReport.cpp
)

target_include_directories(PlagiarismCheckerGUI PRIVATE
    ../src
)

target_link_libraries(PlagiarismCheckerGUI PRIVATE
    Qt6::Widgets
)
