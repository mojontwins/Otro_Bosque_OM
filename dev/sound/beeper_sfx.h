// MT MK3 OM v0.4 [Cheril in Otro Bosque]
// Copyleft 2017, 2018 by The Mojon Twins

// beeper.h
// Cointains Beeper sound effects

// Most effects have been taken off beep_fx's demo project.
// So I guess they should be credited to Shiru again ;)

// Adapted by na_th_an

extern unsigned char asm_beepfx [0];

#asm

	;BeepFX player by Shiru
	;You are free to do whatever you want with this code

	.sound_play
		ld hl,sfxData	;address of sound effects data

		di
		push ix
		push iy

		ld b,0
		ld c,a
		add hl,bc
		add hl,bc
		ld e,(hl)
		inc hl
		ld d,(hl)
		push de
		pop ix			;put it into ix

		xor a
		ld (sfxRoutineToneBorder  +1),a
		ld (sfxRoutineNoiseBorder +1),a
		ld (sfxRoutineSampleBorder+1),a


	.readData
		ld a,(ix+0)		;read block type
		ld c,(ix+1)		;read duration 1
		ld b,(ix+2)
		ld e,(ix+3)		;read duration 2
		ld d,(ix+4)
		push de
		pop iy

		dec a
		jr z,sfxRoutineTone
		dec a
		jr z,sfxRoutineNoise
		dec a
		jr z,sfxRoutineSample
		pop iy
		pop ix
		ei
		ret

		

	;play sample

	.sfxRoutineSample
		ex de,hl
	.sfxRS0
		ld e,8
		ld d,(hl)
		inc hl
	.sfxRS1
		ld a,(ix+5)
	.sfxRS2
		dec a
		jr nz,sfxRS2
		rl d
		sbc a,a
		and 16
	.sfxRoutineSampleBorder
		or 0
		out (254),a
		dec e
		jr nz,sfxRS1
		dec bc
		ld a,b
		or c
		jr nz,sfxRS0

		ld c,6
		
	.nextData
		add ix,bc		;skip to the next block
		jr readData



	;generate tone with many parameters

	.sfxRoutineTone
		ld e,(ix+5)		;freq
		ld d,(ix+6)
		ld a,(ix+9)		;duty
		ld (sfxRoutineToneDuty+1),a
		ld hl,0

	.sfxRT0
		push bc
		push iy
		pop bc
	.sfxRT1
		add hl,de
		ld a,h
	.sfxRoutineToneDuty
		cp 0
		sbc a,a
		and 16
	.sfxRoutineToneBorder
		or 0
		out (254),a

		dec bc
		ld a,b
		or c
		jr nz,sfxRT1

		ld a,(sfxRoutineToneDuty+1)	 ;duty change
		add a,(ix+10)
		ld (sfxRoutineToneDuty+1),a

		ld c,(ix+7)		;slide
		ld b,(ix+8)
		ex de,hl
		add hl,bc
		ex de,hl

		pop bc
		dec bc
		ld a,b
		or c
		jr nz,sfxRT0

		ld c,11
		jr nextData



	;generate noise with two parameters

	.sfxRoutineNoise
		ld e,(ix+5)		;pitch

		ld d,1
		ld h,d
		ld l,d
	.sfxRN0
		push bc
		push iy
		pop bc
	.sfxRN1
		ld a,(hl)
		and 16
	.sfxRoutineNoiseBorder
		or 0
		out (254),a
		dec d
		jr nz,sfxRN2
		ld d,e
		inc hl
		ld a,h
		and 31
		ld h,a
	.sfxRN2
		dec bc
		ld a,b
		or c
		jr nz,sfxRN1

		ld a,e
		add a,(ix+6)	;slide
		ld e,a

		pop bc
		dec bc
		ld a,b
		or c
		jr nz,sfxRN0

		ld c,7
		jr nextData


	.sfxData

	.SoundEffectsData
		defw SoundEffect0Data
		defw SoundEffect1Data
		defw SoundEffect2Data
		defw SoundEffect3Data
		defw SoundEffect4Data
		defw SoundEffect5Data
		defw SoundEffect6Data

	.SoundEffect0Data
		defb 1 ;tone
		defw 72,20,500,2,128
		defb 0
	.SoundEffect1Data
		defb 1 ;tone
		defw 10,100,2000,100,128
		defb 0
	.SoundEffect2Data
		defb 2 ;noise
		defw 3,2000,32776
		defb 0
	.SoundEffect3Data
		defb 1 ;tone
		defw 1,400,300,65511,128
		defb 2 ;noise
		defw 1,3000,5270
		defb 0
	.SoundEffect4Data
		defb 2 ;noise
		defw 1,1000,4
		defb 1 ;tone
		defw 1,1000,2000,0,128
		defb 0
	.SoundEffect5Data
		defb 1 ;tone
		defw 2,4000,400,200,64
		defb 1 ;tone
		defw 2,4000,200,200,32
		defb 0
	.SoundEffect6Data
		defb 2 ;noise
		defw 5,1000,5
		defb 0		

	._asm_beepfx defb 0
#endasm

void beep_fx (unsigned char n) {
	// Cargar en A el valor de n
	asm_beepfx [0] = n;
	#asm
		push ix
		push iy
		ld a, (_asm_beepfx)
		call sound_play
		pop ix
		pop iy
	#endasm
}
