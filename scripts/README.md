# pcm
Script used for PCM sound program by Respberry Pi Pico, written in PHP.

ここには、WAVEファイルをC用のヘッダファイルに変換するためのスクリプトを置いています。実行には、PHPが必要です。

## Contents in this directory
convert16k8.php: Convert *.wav (16000 Hz, mono, 8 bit) in current directory to text files (*.wav.txt) containing C array string

convert16k9.phpを実行すると、カレントディレクトリーにあるWAVEファイル(16000 Hz, mono, 8 bit)を、Cの配列用の文字列として、テキストデーターに変換します。

## License
LGPL v2.1