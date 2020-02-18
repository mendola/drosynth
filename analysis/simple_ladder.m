

stage = zeros(1,4);
output = 0;
input = 0;
gain_compensation = 0.2;
function output_samples = simple_ladder(samples, cutoff_hz, Fs)
stage = zeros(1,4);
stage_Z1 = zeros(1,4);
stage_tanh = zeros(1,3);
output = 0;
input = 0;
gain_compensation = 0.2;
    output_samples = zeros(1,length(samples));
    #Normalized cutoff (0, 1) in radians: ((2*pi) * cutoff / samplerate)
    gain = (2 * pi) * cutoff_hz / Fs;
    gain = gain * pi / 1.3;  #correction factor that allows _cutoff to be supplied Hertz
    # FIR part with gain g
    h = gain / 1.3;
    h0 = gain * 0.3 / 1.3;
    resonance = 0   
 
    for i = 1:length(samples)
        for stage_idx = 1:4
            if stage_idx ~= 1
                input = stage(stage_idx-1);
                stage_tanh(stage_idx-1) = tanh(input);
                if stage_idx ~= 4
                    factor = stage_tanh(stage_idx);
                else
                    factor = tanh(stage_Z1(stage_idx));
                end
                stage(stage_idx) = (h* stage_Z1(stage_idx) + h0 * stage_tanh(stage_idx - 1)) + (1.0 - gain) * factor;
            else 
                # First stage:
                input = samples(i) - ((4.0 * resonance) * (output - gain_compensation * samples(i)));
                stage(stage_idx) = (h * tanh(input) + h0 * stage_Z1(stage_idx)) + (1.0 - gain) * stage_tanh(stage_idx);
            end
            stage_Z1(stage_idx) = stage(stage_idx);
        end
        
        output = stage(4);
        if (output < 0.000001 && output > -0.000001)
            output = 0
        end
        
        output_samples(i) = output;
    end
end
    