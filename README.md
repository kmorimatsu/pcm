# pcm
PCM sound by Respberry Pi Pico

Raspberry Pi Picoを用いたPCMサウンドの再生サンプルコードです。16000 Hz, 8 bitのデーターを、再生することができます。

## PCM sound output
GP28

サウンドは、GP28に出力されます。イヤホンを直接GP32に接続すれば、再生音を聞くことができます。アンプにつなげる場合は、shematic.pngを参考にして、ローパスフィルターを通して接続することをお勧めします。

## License
LGPL v2.1 except for wav.h

サウンドの生データーを含むヘッダファイルのライセンスに関しては、それぞれのファイルを参照して下さい。その他のファイルのライセンスは、LGPLです。