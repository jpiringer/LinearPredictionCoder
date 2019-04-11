# new load path for dylibs

install_name_tool -change /usr/local/lib/libsndfile.1.dylib @loader_path/../Frameworks/libsndfile.1.dylib ~/Library/Audio/Plug-Ins/VST3/LinearPredictionCoder.vst3/Contents/MacOS/LinearPredictionCoder

install_name_tool -change /usr/local/lib/libsndfile.1.dylib @loader_path/../Frameworks/libsndfile.1.dylib ~/Library/Audio/Plug-Ins/Components/LinearPredictionCoder.component/Contents/MacOS/LinearPredictionCoder

rm -r ./deploy
mkdir ./deploy
cp -r ~/Library/Audio/Plug-Ins/Components/LinearPredictionCoder.component ./deploy/LinearPredictionCoder.component
cp -r ~/Library/Audio/Plug-Ins/VST3/LinearPredictionCoder.vst3 ./deploy/LinearPredictionCoder.vst3
cd deploy
zip LinearPredictionCoder-AU.zip -r ./LinearPredictionCoder.component/
zip LinearPredictionCoder-VST3.zip -r ./LinearPredictionCoder.vst3/
