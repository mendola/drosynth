from scipy.signal import freqz, medfilt
import numpy as np
from math import pi, tanh, ceil
import matplotlib.pyplot as plt
from control import mag2db
import pdb

def moving_avg(samples, windowsizee):
    output = []
    curr_output = 0.0
    for x in samples:
        windowsize = windowsizee/1000 + 1
        curr_output = (float(x) * (1.0/float(windowsize)) + curr_output * float(windowsize - 1) / float(windowsize))
        output.append(curr_output)
    return output

def simple_ladder(samples, cutoff_hz, Fs):
    stage = [0,0,0,0]
    stage_Z1 = [0,0,0,0]
    stage_tanh = [0,0,0]
    output = 0
    input = 0
    gain_compensation = 0.2
    output_samples = [0 for i in range(len(samples))]
    #Normalized cutoff [0, 1] in radians: [[2*pi] * cutoff / samplerate]
    gain = (2 * pi) * cutoff_hz / Fs
    gain = gain * pi / 1.3  #correction factor that allows _cutoff to be supplied Hertz
    # FIR part with gain g
    h = gain / 1.3
    h0 = gain * 0.3 / 1.3
    resonance = 0
    
    for i in range(len(samples)):
        for stage_idx in range(4):
            if stage_idx != 0:
                input = stage[stage_idx-1]
                stage_tanh[stage_idx-1] = tanh(input)
                if stage_idx != 3:
                    factor = stage_tanh[stage_idx]
                else:
                    factor = tanh(stage_Z1[stage_idx])
                stage[stage_idx] = (h* stage_Z1[stage_idx] + h0 * stage_tanh[stage_idx - 1]) + (1.0 - gain) * factor
            else:
                # First stage:
                input = samples[i] - ((4.0 * resonance) * (output - gain_compensation * samples[i]))
                stage[stage_idx] = (h * tanh(input) + h0 * stage_Z1[stage_idx]) + (1.0 - gain) * stage_tanh[stage_idx]
            stage_Z1[stage_idx] = stage[stage_idx]
        
        output = stage[3]
        if (output < 0.000001 and output > -0.000001):
            output = 0
        
        output_samples[i] = output
    return output_samples
    
if __name__ == "__main__":
    impulse = [1] + [0]*500
    #cutoffs = np.linspace(7000,12000,10)
    cutoffs = np.linspace(20,22000,20)
    fig1 = plt.figure()
    fig2 = plt.figure()
    num_subplots = len(cutoffs)
    MAX_ROWS_PER_COLUMN = 5
    NUM_COLUMNS = min(num_subplots, MAX_ROWS_PER_COLUMN)
    NUM_ROWS = ceil(num_subplots / NUM_COLUMNS)
    plot_id = 0
    for cutoff in cutoffs:
        plot_id += 1
        L = 50;           #Number of taps in the filter.
        omega_c = 0.25;   #Center frequency of the passband (2pi*rad/sample)

        #Define the filter coefficients and get their response.
       # h = 2/L * np.cos( 2*pi * omega_c * np.linspace( 0, L -1, L-1) )
        h = simple_ladder(impulse, cutoff, 44100)
        #h = [1,1] + [0] * 498
        #h = moving_avg(impulse, cutoff)
        plt.figure(fig1.number)
        curr_ax = plt.subplot(NUM_COLUMNS, NUM_ROWS, plot_id)
        plt.stem(h,use_line_collection=True, markerfmt=" ", label="Cutoff: %d Hz" % (cutoff))
        curr_ax.legend()
        plt.grid(True)
        
        # Plot freqz
        plt.figure(fig2.number)
        curr_ax = plt.subplot(NUM_COLUMNS, NUM_ROWS, plot_id)
        curr_ax.set_title('Digital filter frequency response')
        w, h = freqz(h, 1, 5001)
        angles = np.linspace(20,22500, 5001)
        #angles = np.unwrap(np.angle(h))
        plt.plot(angles, np.abs(h), 'g', label="Cutoff: %d Hz" % (cutoff))
        plt.semilogy(np.linspace(0,22500, 5001),abs(h))
        # plt.xlabel('Frequency (Hz)')
        # plt.ylabel('Magnitude (dB)')
        curr_ax.legend()
        
        plt.grid(True)

    plt.show()