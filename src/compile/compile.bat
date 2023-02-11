@ECHO OFF
REM This script will compile fastTextWeb using Emscripten.
REM This will not download & install Emscripten, but it must be activated.
REM In order to download & install Emscripten, visit: https://emscripten.org/docs/getting_started/Tutorial.html

REM Navigate into the 'src' directory
cd ../

REM Compile the C++ 11 files and output the WebAssembly files 
REM Optimization level 2 is being used, which hides some debugging/assertion information
REM Append next flags for debug mode -g -s ASSERTIONS -sNO_DISABLE_EXCEPTION_CATCHING -lembind -sEXPORT_EXCEPTION_HANDLING_HELPERS
REM Use getExceptionMessage in JS to get message from error code https://emscripten.org/docs/porting/Debugging.html
emcc --bind -o ../fasttext-wasm.js args.cc matrix.cc dictionary.cc loss.cc productquantizer.cc densematrix.cc quantmatrix.cc vector.cc model.cc utils.cc meter.cc fasttext.cc main.cc -s NO_EXIT_RUNTIME -s MODULARIZE=1 -s EXPORT_NAME='fasttextmodule' --embed-file lid.176.ftz -std=c++11 -O2