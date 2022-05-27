# pcm
PCM sound by Respberry Pi Pico to play 16000 Hz, 8 bit or 12000 Hz, 12 bit audio data

Raspberry Pi Picoを用いたPCMサウンドの再生サンプルコードです。16000 Hz, 8 bitもしくは12000 Hz, 12 bitのデーターを、再生することができます。

## PCM sound output
GP28. If earphone is connected directory to GP28, you can hear the sound. When an amplifier will be connected, referer the schematic for the connection of low-pass filter.

サウンドは、GP28に出力されます。イヤホンを直接GP28に接続すれば、再生音を聞くことができます。アンプにつなげる場合は、shematic.pngを参考にして、ローパスフィルターを通して接続することをお勧めします。
![schematic.png](https://raw.githubusercontent.com/kmorimatsu/pcm/main/schematic.png)

## How to use
Copy and modify the source code for your purpose. If the code used as it is, Raspberry Pi Pico says "pan yaku" ("bake bread" in Japanese) in the beginning. After 10 minutes, it says "pan yaketa" ("bread is baked" in Japanese) several times.

使い方：ソースコードをコピー・修正して用いてください。ここのコードをそのまま利用すれば、Raspberry Pi Pico は始めに「パン焼く」と言い、10分後に「パン焼けた」を何回か繰り返します。

## How to embed sound file
Prepare either 16000 Hz, 8 bit, mono, or 12000 Hz 16 bit mono wave file (\*.wav). Use the php-script placed in scripts folder to convert the wave file to text file (\*.txt). Use the text file to construct C array in source code.

About 130 seconds of 16000 Hz 8 bit sound can be embded. About 116 seconds of 12000 Hz 16 bit sound can be embded (it will be converted to 12 bit).

サウンドファイルをソースコードに埋め込むには、まず、16000 Hz, 8 bit,モノラル、もしくは12000 Hz 16 bit, モノラルのwaveファイルを準備してください。次に、scriptsディレクトリーにあるPHPスクリプトを使って、テキストファイルに変換します。テキストファイルは、Cコード中の配列表記として埋め込むことができます。

16000 Hz 8 bitのサウンドなら、約130秒埋め込むことができます。12000 Hz 16 bitなら、約116秒です（12 bitサウンドに変換されます）。

## License
Public domain except for header files

サウンドの生データーを含むヘッダファイルのライセンスに関しては、それぞれのファイルを参照して下さい。その他のファイルのライセンスは、パブリックドメインです。ご自由にお使いください。