%macro export_func 1
   global _%1
   _%1:
%endmacro
%define USE_SECTIONS
%define SAMPLE_RATE	44100
%define MAX_INSTRUMENTS	10
%define MAX_VOICES 1
%define HLD 1
%define BPM 120.000000
%define MAX_PATTERNS 59
%define PATTERN_SIZE_SHIFT 4
%define PATTERN_SIZE (1 << PATTERN_SIZE_SHIFT)
%define	MAX_TICKS (MAX_PATTERNS*PATTERN_SIZE)
%define	SAMPLES_PER_TICK 5512
%define DEF_LFO_NORMALIZE 0.0000453515
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
%define 	GO4K_USE_VCO_MOD_PM
%define 	GO4K_USE_VCO_MOD_TM
%define 	GO4K_USE_VCO_MOD_DM
%define 	GO4K_USE_VCF_CHECK
%define 	GO4K_USE_VCF_MOD_FM
%define 	GO4K_USE_VCF_HIGH
%define 	GO4K_USE_VCF_BAND
%define 	GO4K_USE_VCF_PEAK
%define 	GO4K_USE_DST_CHECK
%define 	GO4K_USE_DST_SH
%define 	GO4K_USE_DST_MOD_DM
%define		GO4K_USE_DLL_CHORUS
%define		GO4K_USE_DLL_CHORUS_CLAMP
%define 	GO4K_USE_DLL_DAMP
%define 	GO4K_USE_DLL_DC_FILTER
%define 	GO4K_USE_FSTG_CHECK
%define		GO4K_USE_WAVESHAPER_CLIP
%define 	GO4K_USE_DLL_MOD
%define 	GO4K_USE_DLL_MOD_PM
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
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	42, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 0, 0, 42, 0, 
	db	42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	42, 0, 42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 42, 0, 42, 0, 
	db	42, 0, 0, 42, 0, 0, 0, 0, 42, 0, 0, 42, 42, 0, 42, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 42, 0, 42, 0, 
	db	42, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 
	db	42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 
	db	42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 0, 42, 42, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 0, 
	db	0, 0, 0, 42, 0, 0, 42, 0, 42, 0, 0, 42, 0, 0, 0, 0, 
	db	0, 0, 0, 0, 0, 0, 0, 0, 42, 0, 0, 42, 0, 0, 0, 0, 
	db	42, HLD, 42, HLD, 45, 42, HLD, 42, 47, HLD, 48, 47, HLD, 47, 48, HLD, 
	db	42, HLD, 42, HLD, 45, 42, HLD, 42, 47, HLD, 48, 47, HLD, 47, 45, HLD, 
	db	42, HLD, 54, HLD, 45, HLD, 53, HLD, 42, HLD, 0, 48, HLD, 0, 48, HLD, 
	db	54, 42, 66, 0, 42, 51, 42, 72, 42, 42, 42, 71, 54, 42, 57, 0, 
	db	54, 42, 66, 0, 42, 51, 42, 71, 42, 42, 42, 69, 54, 42, 57, 0, 
	db	54, 42, 66, 0, 42, 51, 42, 71, 42, 42, 42, 71, 54, 42, 57, 0, 
	db	42, HLD, 66, HLD, 42, 42, HLD, 0, 54, HLD, 54, HLD, 42, 42, HLD, 0, 
	db	42, HLD, 66, HLD, 42, 42, HLD, 0, 54, HLD, 54, HLD, 42, HLD, 42, HLD, 
	db	42, HLD, 66, HLD, 42, 42, HLD, 0, 67, HLD, 66, HLD, 43, HLD, 42, HLD, 
	db	42, HLD, 42, HLD, 42, HLD, 42, 42, 45, 45, 45, 47, HLD, 47, 47, HLD, 
	db	42, HLD, 42, HLD, 42, HLD, 42, 42, 48, 48, 48, 47, HLD, 47, 47, HLD, 
	db	42, HLD, 42, HLD, 42, HLD, 42, 42, 48, HLD, 0, 45, HLD, HLD, 47, HLD, 
	db	54, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	54, HLD, 0, 0, 0, 0, 0, 0, 59, HLD, 0, 0, 0, 0, 0, 0, 
	db	52, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	30, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	33, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	49, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	59, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	62, HLD, 0, 0, 0, 0, 0, 0, 64, HLD, 0, 0, 0, 0, 0, 0, 
	db	56, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	42, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	db	41, HLD, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
go4k_patterns_end
%ifdef USE_SECTIONS
section .g4kmuc2 data align=1
%else
section .data
%endif
go4k_pattern_lists
Instrument0List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 3, 2, 1, 2, 3, 2, 1, 2, 3, 2, 4, 5, 5, 5, 5, 5, 5, 5, 5, 2, 3, 2, 4, 2, 3, 2, 4, 5, 5, 5, 5, 5, 5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 
Instrument1List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 2, 3, 2, 7, 8, 9, 8, 9, 8, 9, 8, 9, 0, 3, 2, 7, 0, 3, 2, 7, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 8, 9, 0, 0, 0, 
Instrument2List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 10, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 10, 11, 11, 11, 11, 11, 11, 11, 11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument3List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 0, 0, 0, 0, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 12, 0, 0, 0, 
Instrument5List		db	0, 0, 0, 0, 0, 0, 0, 0, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 13, 14, 13, 15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument6List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 16, 17, 16, 18, 16, 17, 16, 18, 19, 20, 19, 21, 19, 20, 19, 21, 16, 17, 16, 18, 16, 17, 16, 18, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument7List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 23, 22, 24, 22, 23, 22, 24, 0, 0, 0, 0, 0, 0, 0, 0, 22, 23, 22, 24, 22, 23, 22, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument8List		db	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 22, 23, 22, 24, 22, 23, 22, 24, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
Instrument9List		db	25, 26, 25, 0, 27, 0, 25, 0, 25, 26, 25, 0, 27, 0, 25, 0, 25, 26, 25, 0, 27, 0, 25, 0, 28, 0, 29, 28, 28, 0, 29, 28, 25, 26, 25, 0, 27, 0, 25, 0, 28, 0, 29, 28, 28, 0, 29, 28, 25, 26, 25, 0, 27, 0, 30, 0, 0, 0, 0, 
Instrument10List		db	31, 32, 31, 0, 33, 0, 31, 0, 31, 32, 31, 0, 33, 0, 31, 0, 31, 32, 31, 0, 33, 0, 31, 0, 34, 0, 35, 34, 34, 0, 35, 34, 31, 32, 31, 0, 33, 0, 31, 0, 34, 0, 35, 34, 34, 0, 35, 34, 33, 32, 31, 0, 33, 0, 25, 0, 0, 0, 0, 
go4k_pattern_lists_end
%ifdef USE_SECTIONS
section .g4kmuc3 data align=1
%else
section .data
%endif
go4k_synth_instructions
GO4K_BEGIN_CMDDEF(Instrument0)
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
GO4K_BEGIN_CMDDEF(Instrument1)
	db GO4K_ENV_ID
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument2)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument3)
	db GO4K_ENV_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
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
	db GO4K_VCO_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_DLL_ID
	db GO4K_DLL_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument7)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument8)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_DST_ID
	db GO4K_VCF_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_DST_ID
	db GO4K_DLL_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument9)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Instrument10)
	db GO4K_ENV_ID
	db GO4K_VCO_ID
	db GO4K_VCO_ID
	db GO4K_FOP_ID
	db GO4K_FOP_ID
	db GO4K_VCF_ID
	db GO4K_PAN_ID
	db GO4K_OUT_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_ENV_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
	db GO4K_VCO_ID
	db GO4K_FST_ID
	db GO4K_FST_ID
GO4K_END_CMDDEF
GO4K_BEGIN_CMDDEF(Global)
	db GO4K_ACC_ID
	db GO4K_ACC_ID
	db GO4K_FOP_ID
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
	GO4K_ENV	ATTAC(0),DECAY(70),SUSTAIN(0),RELEASE(77),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(87),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DST	DRIVE(96), SNHFREQ(128), FLAGS(0)
	GO4K_VCF	FREQUENCY(64),RESONANCE(64),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(8),RESONANCE(32),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(80), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(55),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(78),DEST(1*MAX_UNIT_SLOTS+1+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument1)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(0),RELEASE(80),GAIN(128)
	GO4K_ENV	ATTAC(0),DECAY(62),SUSTAIN(0),RELEASE(68),GAIN(128)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(64),GAIN(128),FLAGS(NOISE)
	GO4K_VCF	FREQUENCY(128),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(20), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument2)
	GO4K_ENV	ATTAC(0),DECAY(0),SUSTAIN(128),RELEASE(64),GAIN(128)
	GO4K_FOP	OP(FOP_POP)
	GO4K_ENV	ATTAC(0),DECAY(54),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(96),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(96),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(116),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(96),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(0),DECAY(68),SUSTAIN(0),RELEASE(76),GAIN(5)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(127),GAIN(64),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(120),RESONANCE(32),VCFTYPE(LOWPASS)
	GO4K_DST	DRIVE(121), SNHFREQ(128), FLAGS(0)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(64),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(16), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument3)
	GO4K_ENV	ATTAC(0),DECAY(0),SUSTAIN(128),RELEASE(64),GAIN(128)
	GO4K_FOP	OP(FOP_POP)
	GO4K_ENV	ATTAC(0),DECAY(50),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(79),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(94),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(0),DECAY(60),SUSTAIN(0),RELEASE(75),GAIN(5)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(127),GAIN(64),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_DST	DRIVE(104), SNHFREQ(100), FLAGS(0)
	GO4K_VCF	FREQUENCY(118),RESONANCE(118),VCFTYPE(LOWPASS)
	GO4K_DLL	PREGAIN(30),DRY(128),FEEDBACK(30),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(20), AUXSEND(0)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument5)
	GO4K_ENV	ATTAC(7),DECAY(76),SUSTAIN(30),RELEASE(9),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(104),SHAPE(74),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DST	DRIVE(96), SNHFREQ(128), FLAGS(0)
	GO4K_VCF	FREQUENCY(46),RESONANCE(32),VCFTYPE(LOWPASS)
	GO4K_VCF	FREQUENCY(10),RESONANCE(70),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(48), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(50),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(128),DEST(1*MAX_UNIT_SLOTS+2+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument6)
	GO4K_ENV	ATTAC(50),DECAY(80),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(114),SHAPE(44),GAIN(128),FLAGS(TRISAW)
	GO4K_VCF	FREQUENCY(0),RESONANCE(16),VCFTYPE(BANDPASS)
	GO4K_DST	DRIVE(96), SNHFREQ(128), FLAGS(0)
	GO4K_VCF	FREQUENCY(80),RESONANCE(128),VCFTYPE(LOWPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_DLL	PREGAIN(63),DRY(128),FEEDBACK(10),DAMP(128),FREQUENCY(0),DEPTH(0),DELAY(17),COUNT(1)
	GO4K_DLL	PREGAIN(100),DRY(128),FEEDBACK(70),DAMP(123),FREQUENCY(40),DEPTH(64),DELAY(9),COUNT(8)
	GO4K_PAN	PANNING(64)
	GO4K_OUT	GAIN(26), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(80),SUSTAIN(90),RELEASE(11),GAIN(128)
	GO4K_FST	AMOUNT(104),DEST(2*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument7)
	GO4K_ENV	ATTAC(5),DECAY(75),SUSTAIN(20),RELEASE(60),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(32),SHAPE(97),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(83),DETUNE(56),PHASE(32),GATES(0),COLOR(48),SHAPE(64),GAIN(90),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(76),DETUNE(72),PHASE(96),GATES(0),COLOR(36),SHAPE(24),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(127),GAIN(0),FLAGS(NOISE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(0),DECAY(40),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(40),DETUNE(64),PHASE(102),GATES(0),COLOR(38),SHAPE(127),GAIN(128),FLAGS(NOISE)
	GO4K_DST	DRIVE(127), SNHFREQ(32), FLAGS(0)
	GO4K_VCF	FREQUENCY(14),RESONANCE(128),VCFTYPE(BANDPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(32),RESONANCE(64),VCFTYPE(HIGHPASS)
	GO4K_DST	DRIVE(63), SNHFREQ(128), FLAGS(0)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(32),DAMP(100),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_VCF	FREQUENCY(15),RESONANCE(128),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(48)
	GO4K_OUT	GAIN(10), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(10),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(80),DEST(17*MAX_UNIT_SLOTS+0+FST_SET)
	GO4K_FST	AMOUNT(96),DEST(16*MAX_UNIT_SLOTS+2+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument8)
	GO4K_ENV	ATTAC(0),DECAY(64),SUSTAIN(64),RELEASE(64),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(16),GATES(0),COLOR(32),SHAPE(97),GAIN(128),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(69),DETUNE(56),PHASE(48),GATES(0),COLOR(48),SHAPE(64),GAIN(90),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(76),DETUNE(72),PHASE(112),GATES(0),COLOR(36),SHAPE(24),GAIN(128),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_ENV	ATTAC(0),DECAY(60),SUSTAIN(0),RELEASE(0),GAIN(128)
	GO4K_VCO	TRANSPOSE(40),DETUNE(64),PHASE(102),GATES(0),COLOR(38),SHAPE(127),GAIN(128),FLAGS(NOISE)
	GO4K_DST	DRIVE(127), SNHFREQ(32), FLAGS(0)
	GO4K_VCF	FREQUENCY(14),RESONANCE(128),VCFTYPE(BANDPASS)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_VCF	FREQUENCY(32),RESONANCE(64),VCFTYPE(HIGHPASS)
	GO4K_DST	DRIVE(63), SNHFREQ(128), FLAGS(0)
	GO4K_DLL	PREGAIN(64),DRY(128),FEEDBACK(0),DAMP(0),FREQUENCY(0),DEPTH(0),DELAY(9),COUNT(8)
	GO4K_VCF	FREQUENCY(16),RESONANCE(64),VCFTYPE(HIGHPASS)
	GO4K_PAN	PANNING(80)
	GO4K_OUT	GAIN(5), AUXSEND(0)
	GO4K_ENV	ATTAC(0),DECAY(94),SUSTAIN(10),RELEASE(0),GAIN(128)
	GO4K_FST	AMOUNT(80),DEST(15*MAX_UNIT_SLOTS+0+FST_SET)
	GO4K_FST	AMOUNT(96),DEST(14*MAX_UNIT_SLOTS+2+FST_SET+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument9)
	GO4K_ENV	ATTAC(0),DECAY(44),SUSTAIN(80),RELEASE(98),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(44),SHAPE(64),GAIN(64),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(76),DETUNE(64),PHASE(0),GATES(0),COLOR(44),SHAPE(64),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(60),RESONANCE(128),VCFTYPE(LOWPASS)
	GO4K_PAN	PANNING(32)
	GO4K_OUT	GAIN(32), AUXSEND(0)
	GO4K_VCO	TRANSPOSE(24),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(64),GAIN(64),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(74),DEST(2*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
	GO4K_ENV	ATTAC(0),DECAY(0),SUSTAIN(128),RELEASE(93),GAIN(128)
	GO4K_FST	AMOUNT(84),DEST(8*MAX_UNIT_SLOTS+1+FST_SET)
	GO4K_FST	AMOUNT(44),DEST(5*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(127),GAIN(1),FLAGS(NOISE|LFO)
	GO4K_FST	AMOUNT(65),DEST(1*MAX_UNIT_SLOTS+2+FST_ADD)
	GO4K_FST	AMOUNT(63),DEST(2*MAX_UNIT_SLOTS+2+FST_ADD+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Instrument10)
	GO4K_ENV	ATTAC(0),DECAY(44),SUSTAIN(80),RELEASE(98),GAIN(128)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(44),SHAPE(64),GAIN(64),FLAGS(SINE)
	GO4K_VCO	TRANSPOSE(76),DETUNE(64),PHASE(0),GATES(0),COLOR(44),SHAPE(64),GAIN(64),FLAGS(SINE)
	GO4K_FOP	OP(FOP_ADDP)
	GO4K_FOP	OP(FOP_MULP)
	GO4K_VCF	FREQUENCY(60),RESONANCE(128),VCFTYPE(LOWPASS)
	GO4K_PAN	PANNING(96)
	GO4K_OUT	GAIN(32), AUXSEND(0)
	GO4K_VCO	TRANSPOSE(24),DETUNE(64),PHASE(0),GATES(0),COLOR(128),SHAPE(64),GAIN(64),FLAGS(SINE|LFO)
	GO4K_FST	AMOUNT(74),DEST(2*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
	GO4K_ENV	ATTAC(0),DECAY(0),SUSTAIN(128),RELEASE(93),GAIN(128)
	GO4K_FST	AMOUNT(84),DEST(8*MAX_UNIT_SLOTS+1+FST_SET)
	GO4K_FST	AMOUNT(44),DEST(5*MAX_UNIT_SLOTS+4+FST_SET+FST_POP)
	GO4K_VCO	TRANSPOSE(64),DETUNE(64),PHASE(0),GATES(0),COLOR(64),SHAPE(127),GAIN(1),FLAGS(NOISE|LFO)
	GO4K_FST	AMOUNT(65),DEST(1*MAX_UNIT_SLOTS+2+FST_ADD)
	GO4K_FST	AMOUNT(63),DEST(2*MAX_UNIT_SLOTS+2+FST_ADD+FST_POP)
GO4K_END_PARAMDEF
GO4K_BEGIN_PARAMDEF(Global)
	GO4K_ACC	ACCTYPE(AUX)
	GO4K_ACC	ACCTYPE(OUTPUT)
	GO4K_FOP	OP(FOP_ADDP2)
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
	dw 44100
%endif
