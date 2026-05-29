function y = IE_Filter(x, fs, fl, fh);
    % Dealing with improper cases here
    fh = min(fs/2, fh); 
    fl = max(1e-9, fl); 
    
    if fl > fh
      nvm = fh; fh = fl; fl = fh; 
      clear nvm
    endif

    if (abs(fh - fl) < 1) 
      printf("The passband is too small! Retry with a wider bandwidth!\n");
      y = NaN;
      return;
    elseif (abs(fh - fl) > fs/2)
      printf("The passband is too large! Retry with a narrower bandwidth!\n");
      y = NaN;
      return;
    endif
    
    % Main duty 
    pkg load signal;
    if fh == inf
      fh = fs/2;
      b = fir1(ceil(0.01*length(x)), fl./(fs/2), 'high');   
      flim = fh;
    else 
      b = fir1(ceil(0.01*length(x)), [fl./(fs/2), fh./(fs/2)]);
      flim = 1.8*fh;
    endif
     
    y = filter(b, 1, x);
    [h, w] = freqz(b, 1, length(x));
    

    figure(); 
    subplot(3, 1, 1); plot(fs*w./(pi), abs(fft(x))./sqrt(length(x)), 'k-'); hold on; grid on; grid minor; 
    xlim([0 flim]); ylim([0, 1.1*max(abs(fft(x))./sqrt(length(x)))]);
    line([fl fl], [0 1.5*max(abs(fft(x))./sqrt(length(x)))], 'color', 'r'); line([fh fh], [0 1.5*max(abs(fft(x))./sqrt(length(x)))], 'color', 'r');
    xlabel('f/Hz'); ylabel('Input Signal Spectrum')

    subplot(3, 1, 2); 
    plot(fs*w./(2*pi), abs(h), 'g-o', 'linewidth', 1,'markersize', 0.1); hold on; grid on; grid minor; 
    xlim([0 flim]); ylim([0, 1.5*max(abs(h))]);
    line([fl fl], [0 10], 'color', 'r'); line([fh fh], [0 1.5*max(abs(fft(x))./sqrt(length(x)))], 'color', 'r'); 
    xlabel('f/Hz');  ylabel('Filter Response')
    subplot(3, 1, 3); 
    plot(fs*w./(pi), abs(fft(y))./sqrt(length(y)), 'b-'); hold on; grid on; grid minor; 
    xlim([0 flim]); ylim([0, 1.1*max(abs(fft(x))./sqrt(length(x)))]);
    line([fl fl], [0 1.5*max(abs(fft(x))./sqrt(length(x)))], 'color', 'r'); line([fh fh], [0 1.5*max(abs(fft(x))./sqrt(length(x)))], 'color', 'r'); 
    xlabel('f/Hz'); ylabel('Output Signal Spectrum')
    

end
[Joy,fs]=audioread("your full name.wav")
fl = 300;
fh = 440;
y = IE_Filter(Joy, fs, fl, fh);
sound(y, fs);
audiowrite('Filter2.wav', y, fs);