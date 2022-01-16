/*
 * SoundController.cpp
 *
 * Created: 2016-02-12 오후 2:35:08
 *  Author: Blonix
 */ 

#include "../define.h"


void CSoundController::Init()
{
	StopTime = 0;
	bMelody = false;
	bRest = false;
	MIDI_Cursor = 0;
	nTempo = SND_BASETEMPO;
	nOctave = SND_BASEOCTAVE;
	nRefLength = SND_BASELEN;
	
	pwmToggle = false;
	bNowPlay = false;
	SNDTCNT = 0x0000;
	DDRSND BITSET(SOUNDPIN);
	memset(MIDI, 0, sizeof(MIDI));
}

void CSoundController::CheckEvent()
{
	switch (GlobalEventQueue[0].nID)
	{
	case ETEST2:	
		char	Temp[MAX_MELODYLEN];
		strcpy(Temp, "t120l16brb");
		PlayMelody(Temp);	FlowManager.Delay(30000, ETEST2);	break;
	case ESND_STOP:	StopAll();	break;
	default:	return;
	}
	EventDel();
}

void CSoundController::MakeFrequency()
{
	if(!bNowPlay)	return;
	if(bRest)		return;
	
	if(pwmToggle){		// OFF
		PORTSND BITCLR(SOUNDPIN);
		pwmToggle = false;
	}
	else{				// ON
		PORTSND BITSET(SOUNDPIN);
		pwmToggle = true;
	}
}

void CSoundController::TimeCheck()
{
	if(!bNowPlay)	return;
	
	if(StopTime < FlowManager.MilliTick){
		if(bMelody){
			RestBeep(10);
			ReadMIDI();
		}
		else	StopAll();
	}
}

void CSoundController::PlayMelody(char MIDIstr[])
{
	strcpy(MIDI, MIDIstr);
	MIDI_Cursor = 0;
	nTempo = SND_BASETEMPO;
	nOctave = SND_BASEOCTAVE;
	nRefLength = SND_BASELEN;
	bRest = false;
	
	bMelody = true;
	ReadMIDI();
}

void CSoundController::PlayBeep(uint16_t Key, uint16_t delay)
{
	SNDTCNT =  65535 - (SNDCLOCK / Key) + 1;
	
	StopTime = FlowManager.MilliTick + delay;
	bRest = false;
	bNowPlay = true;
}

void CSoundController::RestBeep(uint16_t delay)
{
	SNDTCNT = 0;
	
	PORTSND BITCLR(SOUNDPIN);
	StopTime = FlowManager.MilliTick + delay;
	bRest = true;
	bNowPlay = true;
}

void CSoundController::StopAll()
{
	PORTSND BITCLR(SOUNDPIN);
	bMelody = false;
	bNowPlay = false;
	SNDTCNT = 0;
}

void CSoundController::ReadMIDI()
{
	uint16_t	TempLen = 0;
	char		TempLenBuff[4] = {0, 0, 0, 0};
	char		TempKey = 0;
	bool		bSharp = false;
	bool		bDot = false;
	
	while(1)
	{
		if(MIDI_Cursor == MAX_MELODYLEN){ StopAll();	return; }
		
		switch ( MIDI[MIDI_Cursor] )
		{
		case 'a':	TempKey = 'a';	break;
		case 'b':	TempKey = 'b';	break;
		case 'c':	TempKey = 'c';	break;
		case 'd':	TempKey = 'd';	break;
		case 'e':	TempKey = 'e';	break;
		case 'f':	TempKey = 'f';	break;
		case 'g':	TempKey = 'g';	break;
		
		case 'l':	TempKey = 'l';	break;		// New RefLen
		case 'o':	TempKey = 'o';	break;		// New Octave
		case 'r':	TempKey = 'r';	break;		// rest
		case 't':	TempKey = 't';	break;		// New Tempo
		case '<':	nOctave--;	MIDI_Cursor++;	continue;
		case '>':	nOctave++;	MIDI_Cursor++;	continue;
		
		default:	StopAll();	return;
		}
		MIDI_Cursor++;
		break;
	}
	
	int i = 0;
	
	while(1)
	{	
		if(MIDI_Cursor == MAX_MELODYLEN){ StopAll();	return; }
			
		if( MIDI[MIDI_Cursor] >='0' && MIDI[MIDI_Cursor] <= '9'){
			TempLenBuff[i] = MIDI[MIDI_Cursor];
			i++;
		}
		else if( MIDI[MIDI_Cursor] == '+' ){
			bSharp = true;
		}
		else if( MIDI[MIDI_Cursor] == '.' ){	// Out loop
			bDot = true;
			MIDI_Cursor++;
			break;
		}
		else{	// Out loop
			break;
		}
		MIDI_Cursor++;
	}
	
	TempLen = (uint16_t)atoi(TempLenBuff);
	if(TempLen == 0)	TempLen = nRefLength;
	if( (TempKey >= 'a') && (TempKey <= 'g') ){
		if(bDot)	TempLen = (60000 / TempLen) * 3 / nTempo;
		else		TempLen = (60000 / TempLen) * 4 / nTempo;
	}
	
	switch ( TempKey )
	{
	case 'a':
		if(bSharp){
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1As, TempLen); return;
			case 2:	PlayBeep(SND_O2As, TempLen); return;
			case 3:	PlayBeep(SND_O3As, TempLen); return;
			case 4:	PlayBeep(SND_O4As, TempLen); return;
			case 5:	PlayBeep(SND_O5As, TempLen); return;
			case 6:	PlayBeep(SND_O6As, TempLen); return;
			case 7:	PlayBeep(SND_O7As, TempLen); return;
			case 8:	PlayBeep(SND_O8As, TempLen); return;
			}
		}
		else{
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1A, TempLen); return;
			case 2:	PlayBeep(SND_O2A, TempLen); return;
			case 3:	PlayBeep(SND_O3A, TempLen); return;
			case 4:	PlayBeep(SND_O4A, TempLen); return;
			case 5:	PlayBeep(SND_O5A, TempLen); return;
			case 6:	PlayBeep(SND_O6A, TempLen); return;
			case 7:	PlayBeep(SND_O7A, TempLen); return;
			case 8:	PlayBeep(SND_O8A, TempLen); return;
			}
		}
		return;
		
	case 'b':
		switch ( nOctave ){
		case 1:	PlayBeep(SND_O1B, TempLen); return;
		case 2:	PlayBeep(SND_O2B, TempLen); return;
		case 3:	PlayBeep(SND_O3B, TempLen); return;
		case 4:	PlayBeep(SND_O4B, TempLen); return;
		case 5:	PlayBeep(SND_O5B, TempLen); return;
		case 6:	PlayBeep(SND_O6B, TempLen); return;
		case 7:	PlayBeep(SND_O7B, TempLen); return;
		case 8:	PlayBeep(SND_O8B, TempLen); return;
		}
		return;
		
	case 'c':
		if(bSharp){
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1Cs, TempLen); return;
			case 2:	PlayBeep(SND_O2Cs, TempLen); return;
			case 3:	PlayBeep(SND_O3Cs, TempLen); return;
			case 4:	PlayBeep(SND_O4Cs, TempLen); return;
			case 5:	PlayBeep(SND_O5Cs, TempLen); return;
			case 6:	PlayBeep(SND_O6Cs, TempLen); return;
			case 7:	PlayBeep(SND_O7Cs, TempLen); return;
			case 8:	PlayBeep(SND_O8Cs, TempLen); return;
			}
		}
		else{
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1C, TempLen); return;
			case 2:	PlayBeep(SND_O2C, TempLen); return;
			case 3:	PlayBeep(SND_O3C, TempLen); return;
			case 4:	PlayBeep(SND_O4C, TempLen); return;
			case 5:	PlayBeep(SND_O5C, TempLen); return;
			case 6:	PlayBeep(SND_O6C, TempLen); return;
			case 7:	PlayBeep(SND_O7C, TempLen); return;
			case 8:	PlayBeep(SND_O8C, TempLen); return;
			}
		}
		return;
	
	case 'd':
		if(bSharp){
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1Ds, TempLen); return;
			case 2:	PlayBeep(SND_O2Ds, TempLen); return;
			case 3:	PlayBeep(SND_O3Ds, TempLen); return;
			case 4:	PlayBeep(SND_O4Ds, TempLen); return;
			case 5:	PlayBeep(SND_O5Ds, TempLen); return;
			case 6:	PlayBeep(SND_O6Ds, TempLen); return;
			case 7:	PlayBeep(SND_O7Ds, TempLen); return;
			case 8:	PlayBeep(SND_O8Ds, TempLen); return;
			}
		}
		else{
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1D, TempLen); return;
			case 2:	PlayBeep(SND_O2D, TempLen); return;
			case 3:	PlayBeep(SND_O3D, TempLen); return;
			case 4:	PlayBeep(SND_O4D, TempLen); return;
			case 5:	PlayBeep(SND_O5D, TempLen); return;
			case 6:	PlayBeep(SND_O6D, TempLen); return;
			case 7:	PlayBeep(SND_O7D, TempLen); return;
			case 8:	PlayBeep(SND_O8D, TempLen); return;
			}
		}
		return;
		
	case 'e':
		switch ( nOctave ){
		case 1:	PlayBeep(SND_O1E, TempLen); return;
		case 2:	PlayBeep(SND_O2E, TempLen); return;
		case 3:	PlayBeep(SND_O3E, TempLen); return;
		case 4:	PlayBeep(SND_O4E, TempLen); return;
		case 5:	PlayBeep(SND_O5E, TempLen); return;
		case 6:	PlayBeep(SND_O6E, TempLen); return;
		case 7:	PlayBeep(SND_O7E, TempLen); return;
		case 8:	PlayBeep(SND_O8E, TempLen); return;
		}
		return;
		
	case 'f':
		if(bSharp){
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1Fs, TempLen); return;
			case 2:	PlayBeep(SND_O2Fs, TempLen); return;
			case 3:	PlayBeep(SND_O3Fs, TempLen); return;
			case 4:	PlayBeep(SND_O4Fs, TempLen); return;
			case 5:	PlayBeep(SND_O5Fs, TempLen); return;
			case 6:	PlayBeep(SND_O6Fs, TempLen); return;
			case 7:	PlayBeep(SND_O7Fs, TempLen); return;
			case 8:	PlayBeep(SND_O8Fs, TempLen); return;
			}
		}
		else{
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1F, TempLen); return;
			case 2:	PlayBeep(SND_O2F, TempLen); return;
			case 3:	PlayBeep(SND_O3F, TempLen); return;
			case 4:	PlayBeep(SND_O4F, TempLen); return;
			case 5:	PlayBeep(SND_O5F, TempLen); return;
			case 6:	PlayBeep(SND_O6F, TempLen); return;
			case 7:	PlayBeep(SND_O7F, TempLen); return;
			case 8:	PlayBeep(SND_O8F, TempLen); return;
			}
		}
		return;
	
	case 'g':
		if(bSharp){
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1Gs, TempLen); return;
			case 2:	PlayBeep(SND_O2Gs, TempLen); return;
			case 3:	PlayBeep(SND_O3Gs, TempLen); return;
			case 4:	PlayBeep(SND_O4Gs, TempLen); return;
			case 5:	PlayBeep(SND_O5Gs, TempLen); return;
			case 6:	PlayBeep(SND_O6Gs, TempLen); return;
			case 7:	PlayBeep(SND_O7Gs, TempLen); return;
			case 8:	PlayBeep(SND_O8Gs, TempLen); return;
			}
		}
		else{
			switch ( nOctave ){
			case 1:	PlayBeep(SND_O1G, TempLen); return;
			case 2:	PlayBeep(SND_O2G, TempLen); return;
			case 3:	PlayBeep(SND_O3G, TempLen); return;
			case 4:	PlayBeep(SND_O4G, TempLen); return;
			case 5:	PlayBeep(SND_O5G, TempLen); return;
			case 6:	PlayBeep(SND_O6G, TempLen); return;
			case 7:	PlayBeep(SND_O7G, TempLen); return;
			case 8:	PlayBeep(SND_O8G, TempLen); return;
			}
		}
		return;
		
	case 'l':	nRefLength = TempLen;	ReadMIDI(); return;		// Recursive
	case 'o':	nOctave = TempLen;		ReadMIDI();	return;		// Recursive
	case 'r':	RestBeep(TempLen);					return;		// Rest
	case 't':	nTempo = TempLen;		ReadMIDI();	return;		// Recursive
	
	default:	StopAll();	return;
	}
}
