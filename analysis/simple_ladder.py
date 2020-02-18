from scipy.signal import freqz
import numpy as np
from math import pi, tanh
import matplotlib.pyplot as plt

import pdb
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
    impulse = [1] + [0]*499
    cutoffs = np.linspace(7000,12000,10)
    fig = plt.figure()
    num_subplots = len(cutoffs)
    MAX_PLOTS_PER_COLUMN = 5
    NUM_COLUMNS = 2
    NUM_ROWS = 5
    plot_id = 0
    for cutoff in cutoffs:
        plot_id += 1
        h = simple_ladder(impulse, cutoff, 44100)
        curr_ax = plt.subplot(NUM_COLUMNS,NUM_ROWS,plot_id)
        curr_ax.title.set_text("Cutoff: %d Hz" % (cutoff))
        plt.stem(h,use_line_collection=True)
        plt.grid(True)

    plt.show()