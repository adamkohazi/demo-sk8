%macro export_func 1
   global _%1
   _%1:
%endmacro
%define USE_SECTIONS
%define SAMPLE_RATE	44100
%define MAX_INSTRUMENTS	12
%define MAX_VOICES 2
%define HLD 1
%define BPM 180.000000
%define MAX_PATTERNS 58
%define PATTERN_SIZE_SHIFT 3
%define PATTERN_SIZE (1 << PATTERN_SIZE_SHIFT)
%define	MAX_TICKS (MAX_PATTERNS*PATTERN_SIZE)
%define	SAMPLES_PER_TICK 7350
%define DEF_LFO_NORMALIZE 0.0000680272
%define	MAX_SAMPLES	(SAMPLES_PER_TICK*MAX_TICKS)
;%define 	GO4K_USE_16BIT_OUTPUT
;%define 	GO4K_USE_GROOVE_PATTERN
;%define 	GO4K_USE_ENVELOPE_RECORDINGS
;%define 	GO4K_USE_NOTE_RECORDINGS
%define 	GO4K_CLIP_OUTPUT
%define 	GO4K_USE_DST
%define 	GO4K_USE_DLL
%define 	GO4K_USE_PAN
%define 	GO4K_USE_GLOBAL_DLL
%define 	GO4K_USE_ENV_CHECK
%define 	GO4K_USE_VCO_CHECK
%define 	GO4K_USE_VCO_PHASE_OFFSET
%define 	GO4K_USE_VCO_SHAPE
%define 	GO4K_USE_VCO_MOD_TM
%define 	GO4K_USE_VCO_MOD_DM
%define 	GO4K_USE_VCO_MOD_CM
%define 	GO4K_USE_VCO_MOD_SM
%define		GO4K_USE_VCO_STEREO
%define 	GO4K_USE_VCF_CHECK
%define 	GO4K_USE_VCF_MOD_FM
%define 	GO4K_USE_VCF_MOD_RM
%define 	GO4K_USE_VCF_HIGH
%define 	GO4K_USE_VCF_BAND
%define 	GO4K_USE_VCF_PEAK
%define		GO4K_USE_VCF_STEREO
%define 	GO4K_USE_DST_CHECK
%define 	GO4K_USE_DST_SH
%define 	GO4K_USE_DST_MOD_SH
%define		GO4K_USE_DST_STEREO
%define		GO4K_USE_DLL_CHORUS
%define		GO4K_USE_DLL_CHORUS_CLAMP
%define 	GO4K_USE_DLL_DAMP
%define 	GO4K_USE_DLL_DC_FILTER
%define 	GO4K_USE_FSTG_CHECK
%define		GO4K_USE_WAVESHAPER_CLIP
%define	MAX_DELAY			65536
%define MAX_UNITS			64
%define	MAX_UNIT_SLOTS	    16
%define GO4K_BEGIN_CMDDEF(def_name)
%define GO4K_END_CMDDEF db 0
%define GO4K_BEGIN_PARAMDEF(def_name)
%define GO4K_END_PARAMDEF
GO4K_ENV_ID		equ		1
%macro	GO4K_ENV 5
	db	%1
	db	%2
	db	%3
	db	%4
	db	%5
%endmacro
%define	ATTAC(val)		val	
%define	DECAY(val)		val	
%define	SUSTAIN(val)	val	
%define	RELEASE(val)	val	
%define	GAIN(val)		val	
struc	go4kENV_val
	.attac		resd	1
	.decay		resd	1
	.sustain	resd	1
	.release	resd	1
	.gain		resd	1
	.size
endstruc
struc	go4kENV_wrk
	.state		resd	1
	.level		resd	1
	.gm			resd	1
	.am			resd	1
	.dm			resd	1
	.sm			resd	1
	.rm			resd	1
	.size
endstruc
%define ENV_STATE_ATTAC		0
%define ENV_STATE_DECAY		1
%define ENV_STATE_SUSTAIN	2
%define ENV_STATE_RELEASE	3
%define ENV_STATE_OFF		4
GO4K_VCO_ID		equ		2
%macro	GO4K_VCO 8
	db	%1
	db	%2
%ifdef GO4K_USE_VCO_PHASE_OFFSET	
	db	%3
%endif	
%ifdef GO4K_USE_VCO_GATE	
	db	%4
%endif	
	db	%5
%ifdef GO4K_USE_VCO_SHAPE	
	db	%6
%endif	
	db	%7
	db	%8
%endmacro
%define	TRANSPOSE(val)	val	
%define	DETUNE(val)		val	
%define	PHASE(val)		val	
%define	GATES(val)		val	
%define	COLOR(val)		val	
%define	SHAPE(val)		val 
%define	FLAGS(val)		val	
%define SINE		0x01
%define TRISAW		0x02
%define PULSE		0x04
%define NOISE		0x08
%define LFO			0x10
%define GATE		0x20
%define	VCO_STEREO	0x40
struc	go4kVCO_val
	.transpose	resd	1
	.detune		resd	1
%ifdef GO4K_USE_VCO_PHASE_OFFSET	
	.phaseofs	resd	1
%endif	
%ifdef GO4K_USE_VCO_GATE	
	.gate		resd	1
%endif	
	.color		resd	1
%ifdef GO4K_USE_VCO_SHAPE	
	.shape		resd	1
%endif	
	.gain		resd	1
	.flags		resd	1	
	.size
endstruc
struc	go4kVCO_wrk
	.phase		resd	1
	.tm			resd	1
	.dm			resd	1
	.fm			resd	1
	.pm			resd	1
	.cm			resd	1
	.sm			resd	1
	.gm			resd	1
	.phase2		resd	1
	.size
endstruc
GO4K_VCF_ID		equ		3
%macro	GO4K_VCF 3
	db	%1
	db	%2
	db	%3
%endmacro
%define LOWPASS		0x1
%define HIGHPASS	0x2
%define BANDPASS	0x4
%define	BANDSTOP	0x3
%define ALLPASS		0x7
%define	PEAK		0x8
%define STEREO		0x10
%define	FREQUENCY(val)	val
%define	RESONANCE(val)	val
%define	VCFTYPE(val)	val
struc	go4kVCF_val
	.freq		resd	1
	.res		resd	1
	.type		resd	1
	.size
endstruc
struc	go4kVCF_wrk
	.low		resd	1
	.high		resd	1
	.band		resd	1
	.freq		resd	1
	.fm			resd	1
	.rm			resd	1
	.low2		resd	1
	.high2		resd	1
	.band2		resd	1
	.size
endstruc
GO4K_DST_ID		equ		4
%macro	GO4K_DST 3
	db	%1
%ifdef GO4K_USE_DST_SH
	db	%2
%ifdef GO4K_USE_DST_STEREO
	db	%3
%endif
%else
%ifdef GO4K_USE_DST_STEREO
	db	%3
%endif
%endif
%endmacro
%define	DRIVE(val)		val
%define	SNHFREQ(val)	val
%define	FLAGS(val)		val
struc	go4kDST_val
	.drive		resd	1
%ifdef GO4K_USE_DST_SH	
	.snhfreq	resd	1
%endif	
	.flags		resd	1
	.size
endstruc
struc	go4kDST_wrk
	.out		resd	1
	.snhphase	resd	1
	.dm			resd	1
	.sm			resd	1
	.out2		resd	1
	.size
endstruc
GO4K_DLL_ID		equ		5
%macro	GO4K_DLL 8
	db	%1
	db	%2
	db	%3
%ifdef GO4K_USE_DLL_DAMP	
	db	%4
%endif		
%ifdef GO4K_USE_DLL_CHORUS	
	db	%5
	db	%6
%endif
	db	%7
	db	%8
%endmacro
%define PREGAIN(val)	val
%define	DRY(val)		val
%define	FEEDBACK(val)	val
%define	DEPTH(val)		val
%define DAMP(val)		val
%define	DELAY(val)		val
%define	COUNT(val)		val
struc	go4kDLL_val
	.pregain	resd	1
	.dry		resd	1
	.feedback	resd	1
%ifdef GO4K_USE_DLL_DAMP	
	.damp		resd	1	
%endif
%ifdef GO4K_USE_DLL_CHORUS
	.freq		resd	1
	.depth
%endif
	.delay		resd	1
	.count		resd	1
	.size
endstruc
struc	go4kDLL_wrk
	.index		resd	1
	.store		resd	1
	.dcin		resd	1
	.dcout		resd	1
%ifdef GO4K_USE_DLL_CHORUS
	.phase		resd	1
%endif
	.buffer		resd	MAX_DELAY
	.size
endstruc
struc	go4kDLL_wrk2
 .pm			resd	1
 .fm			resd	1
 .im			resd	1
 .dm			resd	1
 .sm			resd	1
 .am			resd	1
 .size
endstruc
GO4K_FOP_ID	equ			6
%macro	GO4K_FOP 1
	db	%1
%endmacro
%define	OP(val)			val
%define FOP_POP			0x1
%define FOP_ADDP		0x2
%define FOP_MULP		0x3
%define FOP_PUSH		0x4
%define FOP_XCH			0x5
%define FOP_ADD			0x6
%define FOP_MUL			0x7
%define FOP_ADDP2		0x8
%define FOP_LOADNOTE	0x9
%define FOP_MULP2		0xa
struc	go4kFOP_val
	.flags		resd	1
	.size
endstruc
struc	go4kFOP_wrk
	.size
endstruc
GO4K_FST_ID		equ		7
%macro	GO4K_FST 2
	db	%1
	dw	%2
%endmacro
%define	AMOUNT(val)		val
%define	DEST(val)		val
%define	FST_SET			0x0000
%define	FST_ADD			0x4000
%define	FST_POP			0x8000
struc	go4kFST_val
	.amount		resd	1
	.op1		resd	1
	.size
endstruc
struc	go4kFST_wrk
	.size
endstruc
GO4K_PAN_ID		equ		8
%macro	GO4K_PAN 1
%ifdef GO4K_USE_PAN
	db	%1
%endif
%endmacro
%define	PANNING(val)	val
struc	go4kPAN_val
%ifdef GO4K_USE_PAN
	.panning	resd	1
%endif
	.size
endstruc
struc	go4kPAN_wrk
	.pm			resd	1
	.size
endstruc
GO4K_OUT_ID		equ		9
%macro	GO4K_OUT 2
	db	%1
%ifdef GO4K_USE_GLOBAL_DLL	
	db	%2
%endif	
%endmacro
%define	AUXSEND(val)	val
struc	go4kOUT_val
	.gain		resd	1
%ifdef GO4K_USE_GLOBAL_DLL	
	.auxsend	resd	1
%endif
	.size
endstruc
struc	go4kOUT_wrk
	.am			resd	1
	.gm			resd	1
	.size
endstruc
GO4K_ACC_ID		equ		10
%macro	GO4K_ACC 1
	db	%1
%endmacro
%define OUTPUT			0
%define	AUX				8
%define ACCTYPE(val)	val
struc	go4kACC_val
	.acctype	resd	1
	.size
endstruc
struc	go4kACC_wrk
	.size
endstruc
%ifdef GO4K_USE_FLD
GO4K_FLD_ID	equ		11
%macro	GO4K_FLD 1
	db	%1
%endmacro
%define	VALUE(val)	val
struc	go4kFLD_val
	.value		resd	1
	.size
endstruc
struc	go4kFLD_wrk
	.vm			resd	1
	.size
endstruc
%endif
%ifdef GO4K_USE_GLITCH
GO4K_GLITCH_ID		equ		12
%macro	GO4K_GLITCH 5
	db	%1
	db	%2
	db	%3
	db	%4
	db	%5
%endmacro
%define	ACTIVE(val)		val
%define	SLICEFACTOR(val)val
%define	PITCHFACTOR(val)val
%define	SLICESIZE(val)	val
struc	go4kGLITCH_val
	.active		resd	1
	.dry		resd	1
	.dsize		resd	1
	.dpitch		resd	1
	.slicesize	resd	1
	.size
endstruc
struc	go4kGLITCH_wrk
	.index		resd	1
	.store		resd	1
	.slizesize	resd	1
	.slicepitch	resd	1
	.unused		resd	1
	.buffer		resd	MAX_DELAY
	.size
endstruc
struc	go4kGLITCH_wrk2
	.am			resd	1
	.dm			resd	1
	.sm			resd	1
	.pm			resd	1
	.size
endstruc
%endif
%ifdef GO4K_USE_FSTG
GO4K_FSTG_ID	equ		13
%macro	GO4K_FSTG 2
	db	%1
	dw	%2
%endmacro
struc	go4kFSTG_val
	.amount		resd	1
	.op1		resd	1
	.size
endstruc
struc	go4kFSTG_wrk
	.size
endstruc
%endif
struc	go4k_instrument
	.release	resd	1
	.note		resd	1
	.workspace	resd	MAX_UNITS*MAX_UNIT_SLOTS
	.dlloutl	resd	1
	.dlloutr	resd	1
	.outl		resd	1
	.outr		resd	1
	.size
endstruc
struc	go4k_synth
	.instruments	resb	go4k_instrument.size * MAX_INSTRUMENTS * MAX_VOICES
	.global			resb	go4k_instrument.size * MAX_VOICES
	.size
endstruc
%ifdef USE_SECTIONS
section .g4kmuc1 data align=1
%else
section .data align=1
%endif
go4k_patterns
	db	0, 0, 0, 0, 0, 0, 0, 0, 
	db	45, 44, 41, 45, HLD, 44, 41, 45, 
	db	HLD, 44, 41, HLD, 42, 42, 49, HLD, 
	db	HLD, 44, 41, HLD, 0, 0, 0, 0, 
	db	60, 60, HLD, 60, HLD, HLD, 60, 60, 
	db	68, 68, HLD, 68, HLD, HLD, 68, HLD, 
	db	68, 68, HLD, 68, 0, 0, 0, 0, 
	db	53, 53, HLD, 53, HLD, HLD, 53, 53, 
	db	56, 56, HLD, 56, HLD, HLD, 56, 56, 
	db	61, 61, HLD, 61, HLD, HLD, 61, HLD, 
	db	61, 61, HLD, 61, 0, 0, 0, 0, 
	db	77, 0, 77, 0, 77, 0, 77, 0, 
	db	77, 0, 77, 0, 77, 0, 77, HLD, 
	db	84, 0, 84, 0, 84, 0, 84, 0, 
	db	84, 0, 85, 0, 85, 0, 85, HLD, 
	db	84, 0, 85, 0, 0, 0, 0, 0, 
	db	106, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, 0, 0, 0, 0, 
	db	53, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, 0, 50, 0, 51, HLD, 57, HLD, 
	db	36, 36, 0, 0, 0, 0, 36, 0, 
	db	0, 0, 36, 0, 36, 36, 0, 0, 
	db	0, 0, 36, 0, 0, 0, 0, 0, 
	db	36, 0, 0, 0, 0, 0, 36, 0, 
	db	0, 0, 0, 0, 36, 0, 0, 0, 
	db	0, 0, 0, 36, 0, 0, 0, 36, 
	db	0, 36, 0, 36, 0, 0, 0, 36, 
	db	0, 36, 0, 36, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 36, 0, 0, 36, 
	db	0, 36, 0, 0, 36, 0, 0, 0, 
	db	36, 0, 36, 0, 36, 0, 36, 0, 
	db	36, 0, 36, 0, 0, 0, 36, 0, 
	db	36, 0, 36, 0, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 36, 0, 36, 
	db	0, 0, 0, 0, 36, HLD, 0, 0, 
	db	36, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	HLD, HLD, HLD, HLD, HLD, HLD, HLD, HLD, 
	db	0, 0, 0, 0, 36, HLD, HLD, HLD, 
go4k_patterns_end
%ifdef USE_SECTIONS
section .g4kmuc2 data align=1
%else
section .data
%endif
go4k_pattern_lists
Instrument0List		db	1, 2, 1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 3, 1, 2, 1, 2, 1, 2, 1, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument1List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 4, 4, 5, 4, 4, 4, 6, 4, 4, 4, 5, 4, 4, 4, 6, 4, 4, 4, 5, 4, 4, 4, 6, 4, 4, 4, 5, 4, 4, 4, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument2List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 7, 7, 8, 9, 7, 7, 8, 10, 7, 7, 8, 9, 7, 7, 8, 10, 7, 7, 8, 9, 7, 7, 8, 10, 7, 7, 8, 9, 7, 7, 8, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument3List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12, 13, 14, 11, 12, 13, 15, 0, 0, 0, 0, 0, 0, 0, 0, 11, 12, 13, 14, 11, 12, 13, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument4List		db	0, 0, 0, 0, 0, 0, 16, 17, 18, 19, 18, 19, 18, 19, 16, 17, 18, 19, 18, 19, 18, 19, 16, 17, 18, 19, 18, 19, 18, 19, 16, 17, 18, 19, 18, 19, 18, 19, 16, 17, 18, 19, 18, 19, 18, 19, 16, 17, 0, 0, 0, 0, 0, 0, 16, 17, 0, 0, 
Instrument5List		db	0, 0, 0, 0, 0, 0, 0, 0, 20, 21, 20, 21, 20, 21, 20, 22, 20, 21, 20, 21, 20, 21, 20, 22, 20, 21, 20, 21, 20, 21, 20, 22, 20, 21, 20, 21, 20, 21, 20, 22, 20, 21, 20, 21, 20, 21, 20, 22, 23, 0, 23, 0, 23, 0, 23, 0, 0, 0, 
Instrument6List		db	0, 0, 0, 0, 0, 0, 0, 0, 24, 24, 24, 24, 24, 24, 24, 0, 24, 24, 24, 24, 24, 24, 24, 0, 24, 24, 24, 24, 24, 24, 24, 0, 24, 24, 24, 24, 24, 24, 24, 0, 24, 24, 24, 24, 24, 24, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument7List		db	0, 0, 0, 0, 0, 0, 0, 0, 25, 26, 25, 26, 25, 26, 25, 27, 25, 26, 25, 26, 25, 26, 25, 27, 25, 26, 25, 26, 25, 26, 25, 27, 25, 26, 25, 26, 25, 26, 25, 27, 25, 26, 25, 26, 25, 26, 25, 27, 28, 29, 28, 29, 28, 29, 28, 29, 0, 0, 
Instrument8List		db	0, 0, 0, 0, 0, 0, 0, 0, 30, 31, 30, 31, 30, 31, 30, 32, 30, 31, 30, 31, 30, 31, 30, 32, 30, 31, 30, 31, 30, 31, 30, 32, 30, 31, 30, 31, 30, 31, 30, 32, 30, 31, 30, 31, 30, 31, 30, 32, 30, 30, 30, 30, 30, 30, 30, 30, 0, 0, 
Instrument9List		db	0, 0, 0, 0, 0, 0, 0, 0, 33, 34, 33, 34, 33, 34, 33, 0, 33, 34, 33, 34, 33, 34, 33, 0, 33, 34, 33, 34, 33, 34, 33, 0, 33, 34, 33, 34, 33, 34, 33, 0, 33, 34, 33, 34, 33, 34, 33, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument10List		db	35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 35, 36, 36, 36, 0, 0, 0, 0, 0, 0, 
Instrument11List		db	0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 0, 0, 0, 0, 0, 37, 0, 0, 
go4k_pattern_lists_end
%ifdef USE_SECTIONS
section .g4kmuc3 data align=1
%else
section .data
%endif
go4k_synth_instructions
GO4K_BEGIN_CMDDEF(Instrument0)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument1)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument2)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument3)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument4)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument5)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument6)
	db GO4K_ENV_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument7)
	db GO4K_ENV_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument8)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument9)
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument10)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument11)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Global)
	db GO4K_ACC_ID
	db GO4K_DLL_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
go4k_synth_instructions_end
%ifdef USE_SECTIONS
section .g4kmuc4 data align=1
%else
section .data
%endif
go4k_synth_parameter_values
GO4K_BEGIN_PARAMDEF(Instrument0)
	GO4K_ENV	ATTAC(32),DECAY(80),SUSTAIN(0),RELEASE(32),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_FST	AMOUNT(24),DEST(4*MAX_UNIT_SLOTS+6+FST_SET)
	GO4K_FST	AMOUNT(80),DEST(5*MAX_UNIT_SLOTS+4+FST_SET)
	GO4K_VCO	TRANSPOSE(64),DETUNE(65),PHASE(64),GATES(0),COLOR(48),SHAPE(80),GAIN(64),FLAGS(SINE|VCO_STEREO)
	GO4K_VCF	FREQUENCY(32),RESONANCE(96),VCFTYPE(LOWPASS|STEREO)
	GO4K_VCF	FREQUENCY(16),RESONANCE(96),VCFTYPE(HIGHPASS|STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(64), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument1)
	GO4K_ENV	ATTAC(0),DECAY(92),SUSTAIN(0),RELEASE(64),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_FST	AMOUNT(48),DEST(5*MAX_UNIT_SLOTS+5+FST_SET)
	GO4K_FST	AMOUNT(25),DEST(5*MAX_UNIT_SLOTS+6+FST_SET)
	GO4K_FST	AMOUNT(72),DEST(5*MAX_UNIT_SLOTS+2+FST_SET)
	GO4K_VCO	TRANSPOSE(64),DETUNE(56),PHASE(32),GATES(0),COLOR(64),SHAPE(96),GAIN(128),FLAGS(TRISAW|VCO_STEREO)
	GO4K_DST	DRIVE(6), SNHFREQ(128), FLAGS(STEREO)
	GO4K_VCF	FREQUENCY(96),RESONANCE(104),VCFTYPE(LOWPASS|STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(48), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument2)
	GO4K_ENV	ATTAC(0),DECAY(92),SUSTAIN(0),RELEASE(64),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_FST	AMOUNT(48),DEST(5*MAX_UNIT_SLOTS+5+FST_SET)
	GO4K_FST	AMOUNT(25),DEST(5*MAX_UNIT_SLOTS+6+FST_SET)
	GO4K_FST	AMOUNT(72),DEST(5*MAX_UNIT_SLOTS+2+FST_SET)
	GO4K_VCO	TRANSPOSE(64),DETUNE(56),PHASE(32),GATES(0),COLOR(64),SHAPE(96),GAIN(128),FLAGS(TRISAW|VCO_STEREO)
	GO4K_DST	DRIVE(6), SNHFREQ(128), FLAGS(STEREO)
	GO4K_VCF	FREQUENCY(96),RESONANCE(104),VCFTYPE(LOWPASS|STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(48), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument3)
	GO4K_ENV	ATTAC(64),DECAY(64),SUSTAIN(128),RELEASE(64),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_VCO	TRANSPOSE(64),DETUNE(72),PHASE(0),GATES(0),COLOR(96),SHAPE(4),GAIN(128),FLAGS(TRISAW|VCO_STEREO)
	GO4K_VCF	FREQUENCY(86),RESONANCE(128),VCFTYPE(LOWPASS|STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(32), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument4)
	GO4K_ENV	ATTAC(76),DECAY(64),SUSTAIN(112),RELEASE(76),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_VCO	TRANSPOSE(64),DETUNE(72),PHASE(64),GATES(0),COLOR(32),SHAPE(64),GAIN(128),FLAGS(SINE|VCO_STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(8), AUXSEND(0)
	GO4K_VCO	TRANSPOSE(26),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(64),GAIN(128),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(80),DEST(2*MAX_UNIT_SLOTS+5+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument5)
	GO4K_ENV	ATTAC(32),DECAY(70),SUSTAIN(0),RELEASE(77),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(80),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DST	DRIVE(96), SNHFREQ(128), FLAGS(0)
	GO4K_VCF	FREQUENCY(64),RESONANCE(64),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(8),RESONANCE(64),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(80), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(55),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(78),DEST(1*MAX_UNIT_SLOTS+1+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument6)
	GO4K_ENV	ATTAC(0),DECAY(96),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE|LFO)
	GO4K_DST	DRIVE(127), SNHFREQ(16), FLAGS(0)
	GO4K_VCF	FREQUENCY(24),RESONANCE(4),VCFTYPE(PEAK)
	GO4K_VCF	FREQUENCY(32),RESONANCE(8),VCFTYPE(LOWPASS)
	GO4K_DST	DRIVE(1), SNHFREQ(128), FLAGS(0)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(32),DECAY(76),SUSTAIN(0),RELEASE(64),GAIN(32)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(64),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(32), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument7)
	GO4K_ENV	ATTAC(0),DECAY(96),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_ENV	ATTAC(0),DECAY(60),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE|LFO)
	GO4K_VCF	FREQUENCY(24),RESONANCE(4),VCFTYPE(PEAK)
	GO4K_VCF	FREQUENCY(32),RESONANCE(4),VCFTYPE(LOWPASS)
	GO4K_DST	DRIVE(1), SNHFREQ(128), FLAGS(0)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(32),DECAY(64),SUSTAIN(0),RELEASE(64),GAIN(16)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(64),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(48)
	GO4K_OUT	GAIN(32), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument8)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_VCF	FREQUENCY(128),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(96)
	GO4K_OUT	GAIN(16), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument9)
	GO4K_ENV	ATTAC(32),DECAY(76),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(32),DEST(4*MAX_UNIT_SLOTS+5+FST_SET)
	GO4K_FST	AMOUNT(95),DEST(3*MAX_UNIT_SLOTS+6+FST_SET)
	GO4K_VCO	TRANSPOSE(96),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(1),GAIN(128),FLAGS(NOISE)
	GO4K_VCF	FREQUENCY(128),RESONANCE(128),VCFTYPE(BANDPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(32)
	GO4K_OUT	GAIN(8), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument10)
	GO4K_ENV	ATTAC(0),DECAY(86),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_VCF	FREQUENCY(32),RESONANCE(96),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(96),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_VCF	FREQUENCY(64),RESONANCE(32),VCFTYPE(ALLPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(112)
	GO4K_OUT	GAIN(12), AUXSEND(0)
	GO4K_ENV	ATTAC(58),DECAY(89),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(112),DEST(2*MAX_UNIT_SLOTS+4+FST_SET)
	GO4K_FST	AMOUNT(48),DEST(3*MAX_UNIT_SLOTS+4+FST_SET)
	GO4K_FST	AMOUNT(32),DEST(4*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument11)
	GO4K_ENV	ATTAC(32),DECAY(0),SUSTAIN(128),RELEASE(0),GAIN(128)
	GO4K_FOP	OP(FOP_PUSH)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE|VCO_STEREO)
	GO4K_DST	DRIVE(127), SNHFREQ(0), FLAGS(STEREO)
	GO4K_VCF	FREQUENCY(128),RESONANCE(32),VCFTYPE(BANDPASS|STEREO)
	GO4K_FOP	OP(FOP_MULP2)
	GO4K_OUT	GAIN(8), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(78),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(128),DEST(3*MAX_UNIT_SLOTS+3+FST_SET)
	GO4K_FST	AMOUNT(0),DEST(4*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Global)
	GO4K_ACC	ACCTYPE(OUTPUT)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(16),DEPTH(64),DELAY(1),COUNT(8)
	GO4K_FOP	OP(FOP_XCH)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(64),FREQUENCY(16),DEPTH(64),DELAY(9),COUNT(8)
	GO4K_OUT	GAIN(96), AUXSEND(0)
GO4K_END_PARAMDEF
go4k_synth_parameter_values_end
%ifdef USE_SECTIONS
section .g4kmuc5 data align=1
%else
section .data
%endif
%ifdef GO4K_USE_DLL
global _go4k_delay_times
_go4k_delay_times
	dw 0
	dw 1116
	dw 1188
	dw 1276
	dw 1356
	dw 1422
	dw 1492
	dw 1556
	dw 1618
	dw 1140
	dw 1212
	dw 1300
	dw 1380
	dw 1446
	dw 1516
	dw 1580
	dw 1642
%endif
