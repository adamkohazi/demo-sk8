#include "mmsystem.h"
#include "mmreg.h"

#define CHANNELS 2

const WAVEFORMATEX waveFormat {
	.wFormatTag =
#ifdef FLOAT_32BIT
	WAVE_FORMAT_IEEE_FLOAT,
#else
	WAVE_FORMAT_PCM,
#endif
	.nChannels = CHANNELS,
	.nSamplesPerSec = SAMPLE_RATE,
	.nAvgBytesPerSec = SAMPLE_RATE * sizeof(SAMPLE_TYPE) * CHANNELS,
	.nBlockAlign = sizeof(SAMPLE_TYPE) * CHANNELS,
	.wBitsPerSample = sizeof(SAMPLE_TYPE) * 8,
	.cbSize = 0 // No extra format info
};

// Audio buffer for stereo output (interleaved samples)
static SAMPLE_TYPE audioBuffer[MAX_SAMPLES * CHANNELS];

// Windows audio output handles
static HWAVEOUT waveOutHandle{};
static WAVEHDR waveHeader = {
	.lpData = (LPSTR)audioBuffer,
	.dwBufferLength = MAX_SAMPLES * sizeof(SAMPLE_TYPE) * CHANNELS
	// All other fields are zero-initialized by default
};

// Multimedia time structure for querying playback position
static MMTIME waveTime {
	.wType = TIME_SAMPLES
		// Other field is zero-initialized by default
};

#ifdef DEBUG // offset may be introduced due to seeking
static DWORD playbackOffset = 0;
#endif

// Returns the current audio playback time in seconds
__forceinline float GetAudioPlaybackTime() {
	waveOutGetPosition(waveOutHandle, &waveTime, sizeof(MMTIME));

	return float(waveTime.u.sample
#ifdef DEBUG // offset may be introduced due to seeking
	+ playbackOffset
#endif
	) / SAMPLE_RATE;
}

static __forceinline void initAudio() {
	CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)_4klang_render, audioBuffer, 0, nullptr);
	waveOutOpen(&waveOutHandle, WAVE_MAPPER, &waveFormat, 0, 0, CALLBACK_NULL);
	waveOutPrepareHeader(waveOutHandle, &waveHeader, sizeof(waveHeader));
	waveOutWrite(waveOutHandle, &waveHeader, sizeof(waveHeader));
}

#ifdef DEBUG
// Pause playback
static __forceinline void pauseAudio() {
	waveOutPause(waveOutHandle);
}

// Resume playback
static __forceinline void resumeAudio() {
	waveOutRestart(waveOutHandle);
}

void seekAudio(float time) {
	// Clamp time to valid range
	if (time < 0.0f) time = 0.0f;

	const float maxTime = float(MAX_SAMPLES) / SAMPLE_RATE;
	if (time > maxTime) time = maxTime;

	// Convert time to sample index (1 sample = 1 frame = stereo pair)
	DWORD sampleIndex = DWORD(time * SAMPLE_RATE);

	// Update global offset tracker
	playbackOffset = sampleIndex;

	// Stop any current playback
	waveOutReset(waveOutHandle);

	// Unprepare the header if already prepared
	waveOutUnprepareHeader(waveOutHandle, &waveHeader, sizeof(WAVEHDR));

	// Adjust pointer into the audio buffer (2 samples per frame for stereo)
	waveHeader.lpData = (LPSTR)(audioBuffer + sampleIndex * CHANNELS);
	waveHeader.dwBufferLength = (MAX_SAMPLES - sampleIndex) * sizeof(SAMPLE_TYPE) * CHANNELS;

	// Re-prepare and write the new buffer
	waveOutPrepareHeader(waveOutHandle, &waveHeader, sizeof(WAVEHDR));
	waveOutWrite(waveOutHandle, &waveHeader, sizeof(WAVEHDR));
}

// Step playback by seconds (+/-)
void stepAudio(float offset) {
	// Get current sample position
	float currentTime = GetAudioPlaybackTime();

	// Calculate new sample position
	float newSample = GetAudioPlaybackTime() + offset;

	// Clamp to valid range
	if (newSample < 0.0f) newSample = 0.0f;

	const float maxTime = float(MAX_SAMPLES) / SAMPLE_RATE;
	if (newSample > maxTime) newSample = maxTime;

	// Seek to that time
	seekAudio(newSample);
}

#endif