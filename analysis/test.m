source("~/.octaverc")
FS = 44100;
CLIP_DURATION_S = 8*1/440;


dt_ticks = 1:FS*CLIP_DURATION_S

f1 = 440.0;

# sin(sample_idx/samples_per_period *2*pi) = sin(sample_idx / (FS/f1) * 2pi)
signal = sin(dt_ticks/ (FS/f1) *2*pi );

plot(signal)

