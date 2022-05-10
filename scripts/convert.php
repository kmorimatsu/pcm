<?php

date_default_timezone_set('America/Los_Angeles');

foreach(glob('*.wav') as $filename) wav2txt($filename);

exit ("\nAll Done!");

function wav2txt($filename){
	echo "---- $filename ----\n";
	$wav=file_get_contents($filename);
	echo "file size: ".strlen($wav)." bytes\n";

	if (!preg_match('/\x64\x61\x74\x61([\s\S]{4})([\s\S]*)/',$wav,$m)) exit ('unknown error');
	$size=ord($m[1][0])|(ord($m[1][1])<<8)|(ord($m[1][2])<<16)|(ord($m[1][3])<<24);
	//echo "data size: $size bytes\n";
	$body=substr($m[2],0,$size);
	echo "body size: ".strlen($body)." bytes\n";

	$result='';
	for($i=0;$i<strlen($body);$i++){
		$b=ord(substr($body,$i,1));
		if ($b<10) $result.='0x0';
		else $result.='0x';
		$result.=dechex($b).',';
		if (0xf==($i&0x0f)) $result.="\n";
	}

	file_put_contents($filename.'.txt',$result);
	echo $filename.".txt created\n";
}
