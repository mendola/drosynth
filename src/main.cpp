// ---------------------------------------------------------------------------------------

#include <iostream>
#include <cmath>
#include <cassert>
#include <cstddef>
#include "portaudiocpp/PortAudioCpp.hxx"

#include "waveform.hpp"
#include "SineWave.hpp"
#include "squarewave.hpp"
#include "trianglewave.hpp"

// ---------------------------------------------------------------------------------------

// Some constants:
const int NUM_SECONDS = 5;
const double SAMPLE_RATE = 44100.0;
const int FRAMES_PER_BUFFER = 64;
const int TABLE_SIZE = 200;



// ---------------------------------------------------------------------------------------

// main:
int main(int, char *[]);
int main(int, char *[])
{
	float frequency = 0;
	try
	{
		// Create a SineWave object:
		std::cout << "Enter a frequency: " << std::endl;

		std::cin >> frequency;
		std::cout << "Setting up PortAudio..." << std::endl;

		WaveGenerator waveGenerator;//TABLE_SIZE, SAMPLE_RATE, frequency);
		Sinewave sw = Sinewave(SAMPLE_RATE, frequency, 0.425);
		Sinewave sw2 = Sinewave(SAMPLE_RATE, frequency, 0.1);
		Squarewave sq = Squarewave(SAMPLE_RATE, frequency, 0.125);
		TriangleWave tr = TriangleWave(SAMPLE_RATE, frequency, 0.125);

		sw.Detune(2.0);
		sw2.Detune(1.0);
		//sw2.Detune(2);
		tr.Detune(1);
		waveGenerator.AddOscillator(&sw);
		//waveGenerator.AddOscillator(&sw2);
		waveGenerator.AddOscillator(&sq);
		waveGenerator.AddOscillator(&tr);
		waveGenerator.SetAllOscFrequencies(frequency);


		std::cout << "Setting up PortAudio..." << std::endl;

		// Set up the System:
		portaudio::AutoSystem autoSys;
		portaudio::System &sys = portaudio::System::instance();

		// Set up the parameters required to open a (Callback)Stream:
		portaudio::DirectionSpecificStreamParameters outParams(sys.defaultOutputDevice(), 2, portaudio::FLOAT32, false, sys.defaultOutputDevice().defaultLowOutputLatency(), NULL);
		portaudio::StreamParameters params(portaudio::DirectionSpecificStreamParameters::null(), outParams, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff);

		std::cout << "Opening stereo output stream..." << std::endl;

		// Create (and open) a new Stream, using the WaveGenerator::generate function as a callback:
		portaudio::MemFunCallbackStream<WaveGenerator> stream(params, waveGenerator, &WaveGenerator::generate);

		std::cout << "Starting playback for " << NUM_SECONDS << " seconds." << std::endl;

		// Start the Stream (audio playback starts):
		stream.start();

		// Wait for 5 seconds:
		//sys.sleep(NUM_SECONDS * 1000);
		while (frequency > 0.1) {
			std::cout << "Enter a frequency." << std::endl;
			std::cin >> frequency;
			waveGenerator.SetAllOscFrequencies(frequency);
		}

		std::cout << "Closing stream..." <<std::endl;

		// Stop the Stream (not strictly needed as termintating the System will also stop all open Streams):
		stream.stop();

		// Close the Stream (not strictly needed as terminating the System will also close all open Streams):
		stream.close();

		// Terminate the System (not strictly needed as the AutoSystem will also take care of this when it 
		// goes out of scope):
		sys.terminate();

		std::cout << "Test finished." << std::endl;
	}
	catch (const portaudio::PaException &e)
	{
		std::cout << "A PortAudio error occured: " << e.paErrorText() << std::endl;
	}
	catch (const portaudio::PaCppException &e)
	{
		std::cout << "A PortAudioCpp error occured: " << e.what() << std::endl;
	}
	catch (const std::exception &e)
	{
		std::cout << "A generic exception occured: " << e.what() << std::endl;
	}
	catch (...)
	{
		std::cout << "An unknown exception occured." << std::endl;
	}

	return 0;
}


