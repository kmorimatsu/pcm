<?php

date_default_timezone_set('America/Los_Angeles');

foreach(glob('*.wav') as $filename) wav2txt($filename);

exit ("\nAll Done!");

function wav2txt($filename){
	echo "---- $filename ----\n";
	$wav=file_get_contents($filename);
	echo 'File size:     '.strlen($wav)." bytes\n";

	/*
		WAV file information
	*/
	
	if (!preg_match(
		'/\x52\x49\x46\x46'.     // RIFF
		'([\s\S]{4})'.
		'\x57\x41\x56\x45'.      // WAVE
		'\x66\x6d\x74\x20'.      // fmt
		'\x10\x00\x00\x00'.      // Linear PCM
		'\x01\x00'.              // Not encoded
		'(\x01\x00|\0x02\0x00)'. // Mono (1) or stereo (2)
		'([\s\S]{4})'.           // Sampling frequency (16000 Hz: 0x80 0x3e 0x00 0x00)
		'([\s\S]{4})'.           // Bytes in second
		'([\s\S]{2})'.           // Block size (mono 8 bit: 0x01 0x00; mono 16 bit: 0x02 0x00)
		'([\s\S]{2})'.           // Bits in sample (mono 8bit: 0x08 0x00; mono 16 bit: 0x10 0x00)
		'([\s\S]*)/',$wav,$m)) exit ('unknown error');
	echo 'Chunk size:    ',str2int($m[1]),"\n";
	echo 'Mono/stereo:   ',str2int($m[2]),"\n";
	echo 'Sampling Hz:   ',str2int($m[3]),"\n";
	echo 'Bytes/sec:     ',str2int($m[4]),"\n";
	echo 'Block size:    ',str2int($m[5]),"\n";
	echo 'Bit(s)/sample: ',str2int($m[6]),"\n";
	if (1!=str2int($m[2])) exit ('must be mono');
	if (12000!=str2int($m[3])) exit ('must be 12000 Hz');
	if (2!=str2int($m[5])) exit ('must be 16 bit data');
	/*
		Data size information and data
	*/
	if (!preg_match('/\x64\x61\x74\x61([\s\S]{4})([\s\S]*)/',$wav,$m)) exit ('unknown error');
	$size=str2int($m[1]);
	// echo "data size: $size bytes\n";
	$body=substr($m[2],0,$size);
	echo 'Body size:     '.strlen($body)." bytes\n";
	if (2880000<$size) {
		// More than 120 seconds
		echo "Too big!\n";
		return;
	}

	$result='';
	$len=strlen($body);
	$body.="\x00\x00\x00\x00";
	for($i=0;$i<$len;$i+=4){
		// Obtain 16 bit data
		$w=ord(substr($body,$i,1));
		$w|=ord(substr($body,$i+1,1))<<8;
		$w+=32768;
		// Obtain 16 bit data again
		$w2=ord(substr($body,$i+2,1));
		$w2|=ord(substr($body,$i+3,1))<<8;
		$w2+=32768;
		// Convert to 24 bits data containing two 12 bits data
		$w>>=4;
		$w2>>=4;
		$w|=$w2<<12;
		// Split into 3 bytes (little endian)
		for($j=0;$j<3;$j++){
			$b=$w&0xff;
			$w>>=8;
			if ($b<10) $result.='0x0';
			else $result.='0x';
			$result.=dechex($b).',';
		}
		if (0x0c==($i&0x0f)) $result.="\n";
	}

	file_put_contents($filename.'.txt',$result);
	echo $filename.".txt created\n";
}

function str2int($str){
	$res=0;
	for($i=0;$i<strlen($str);$i++){
		$res<<=8;
		$res|=ord(substr($str,strlen($str)-$i-1,1));
	}
	return $res;
}
