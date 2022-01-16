/*
 * SoundController.h
 *
 * Created: 2016-02-12 오후 2:35:20
 *  Author: Blonix
 */ 


#ifndef SOUNDCONTROLLER_H_
#define SOUNDCONTROLLER_H_

//#define SOUNDPIN		PA6			// define.h
//#define MAX_MELODYLEN	100			// define.h
#define DDRSND			DDRA
#define PORTSND			PORTA
#define SNDCLOCK		F_CPU/16		// 16bit timer, freescale /8, /2 Hz

#define SND_BASETEMPO	120
#define SND_BASEOCTAVE	5
#define SND_BASELEN		4

//======================================================================================//
//============================  Sound Controller Class  ================================//
//======================================================================================//


class	CSoundController
{
private:
	char		MIDI[MAX_MELODYLEN];
	
	volatile	uint16_t	MIDI_Cursor;
	volatile	bool		bMelody;
	volatile	bool		bRest;
	volatile	uint8_t		nTempo;			// Tempo of MIDI now
	volatile	uint8_t		nOctave;		// Octave of MIDI now
	volatile	uint8_t		nRefLength;		// Default sound len now
	
	volatile	bool		pwmToggle;
	volatile	bool		bNowPlay;
	volatile	uint32_t	StopTime;
	
	void	ReadMIDI();
	void	RestBeep(uint16_t delay);
	
public:
	volatile	uint16_t	SNDTCNT;

	void	Init();
	void	CheckEvent();
	void	MakeFrequency();						// TimerCall
	void	TimeCheck();							// 1ms TimerCall
	void	PlayMelody(char MIDIstr[]);
	void	PlayBeep(uint16_t Key, uint16_t delay);	// SND_01C ~ SND_08B
	void	StopAll();
};




#define SND_O1C		33
#define SND_O1Cs	35
#define SND_O1D		37
#define SND_O1Ds	39
#define SND_O1E		41
#define SND_O1F		44
#define SND_O1Fs	46
#define SND_O1G		49
#define SND_O1Gs	52
#define SND_O1A		55
#define SND_O1As	59
#define SND_O1B		62

#define SND_O2C		65
#define SND_O2Cs	68
#define SND_O2D		73
#define SND_O2Ds	78
#define SND_O2E		82
#define SND_O2F		87
#define SND_O2Fs	92
#define SND_O2G		98
#define SND_O2Gs	104
#define SND_O2A		110
#define SND_O2As	117
#define SND_O2B		123

#define SND_O3C		131
#define SND_O3Cs	139
#define SND_O3D		147
#define SND_O3Ds	156
#define SND_O3E		165
#define SND_O3F		175
#define SND_O3Fs	185
#define SND_O3G		196
#define SND_O3Gs	208
#define SND_O3A		220
#define SND_O3As	233
#define SND_O3B		247

#define SND_O4C		262
#define SND_O4Cs	277
#define SND_O4D		294
#define SND_O4Ds	311
#define SND_O4E		330
#define SND_O4F		349
#define SND_O4Fs	370
#define SND_O4G		392
#define SND_O4Gs	415
#define SND_O4A		440
#define SND_O4As	466
#define SND_O4B		494

#define SND_O5C		523
#define SND_O5Cs	554
#define SND_O5D		587
#define SND_O5Ds	622
#define SND_O5E		659
#define SND_O5F		698
#define SND_O5Fs	740
#define SND_O5G		784
#define SND_O5Gs	831
#define SND_O5A		880
#define SND_O5As	932
#define SND_O5B		988

#define SND_O6C		1047
#define SND_O6Cs	1109
#define SND_O6D		1175
#define SND_O6Ds	1245
#define SND_O6E		1319
#define SND_O6F		1397
#define SND_O6Fs	1480
#define SND_O6G		1568
#define SND_O6Gs	1661
#define SND_O6A		1760
#define SND_O6As	1865
#define SND_O6B		1976

#define SND_O7C		2093
#define SND_O7Cs	2217
#define SND_O7D		2349
#define SND_O7Ds	2489
#define SND_O7E		2637
#define SND_O7F		2794
#define SND_O7Fs	2960
#define SND_O7G		3136
#define SND_O7Gs	3322
#define SND_O7A		3520
#define SND_O7As	3729
#define SND_O7B		3951

#define SND_O8C		4186
#define SND_O8Cs	4435
#define SND_O8D		4699
#define SND_O8Ds	4978
#define SND_O8E		5274
#define SND_O8F		5588
#define SND_O8Fs	5920
#define SND_O8G		6272
#define SND_O8Gs	6645
#define SND_O8A		7040
#define SND_O8As	7459
#define SND_O8B		7902




#endif /* SOUNDCONTROLLER_H_ */