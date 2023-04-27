%get tables
ps128 = readmatrix('ps128.txt');
ps64 = readmatrix('ps64.txt');
ps32 = readmatrix('ps32.txt');
ps16 = readmatrix('ps16.txt');
ps8 = readmatrix('ps8.txt');
ps4 = readmatrix('ps4.txt');
ps2 = readmatrix('ps2.txt');

%create voltage array
v = linspace(0,253,988);
v = v*5/255;

lim = 988;

%recreate gain error plot from data sheet figure 23-11
figure(1);
y128 = ps128(1:lim, 6)*5/255;
y64 = ps64(1:lim, 6)*5/255;
y32 = ps32(1:lim, 6)*5/255;
y16 = ps16(1:lim, 6)*5/255;
y8 = ps8(1:lim, 6)*5/255;
y4 = ps4(1:lim, 6)*5/255;
y2 = ps2(1:lim, 6)*5/255;

plot(v,y128,'g-',v,y64,'b-',v,y32,'r-',v,y16,'c-',v,y8,'y-',v,y4,'m-',v,y2,'k-',v,v, 'r--');
legend({'128','64','32','16','8','4','2','Ideal'},'Location','northwest');
xlim([0 253]*5/255);
ylim([0 253]*5/255);
xlabel("Ideal ADC signal (V)");
ylabel("Actual ADC signal (V)");
title("Visualisation of Gain Error");

%calc gain error
ge128 = (((ps128(lim, 6))-253)/255)*5;
ge64 = (((ps64(lim, 6))-253)/255)*5;
ge32 = (((ps32(lim, 6))-253)/255)*5;
ge16 = (((ps16(lim, 6))-253)/255)*5;
ge8 = (((ps8(lim, 6))-253)/255)*5;
ge4 = (((ps4(lim, 6))-253)/255)*5;
ge2 = (((ps2(lim, 6))-253)/255)*5;

%plot gain error
figure(2);
error = [ge2,ge4,ge8,ge16,ge32,ge64,ge128];
prescale=[2,4,8,16,32,64,128];
semilogx(prescale,error);
xlim([2 128]);
xticks([2 4 8 16 32 64 128]);
xlabel("ADC Prescale Value");
ylabel("Gain Error (V)");
title("Relationship Between Prescale Value and Gain Error");

%integral non-lineratiy error

%adjusted for gain error for each prescaler
ideal128 = v*(1+ge128/5);
ideal64 = v*(1+ge64/5);
ideal32 = v*(1+ge32/5);
ideal16 = v*(1+ge16/5);
ideal8 = v*(1+ge8/5);
ideal4 = v*(1+ge4/5);
ideal2 = v*(1+ge2/5);

INL128 = 0;
for i = 1:988
    INL = abs(y128(i)-ideal128(i));
    if(INL>INL128)
        INL128 = INL;
    end
end

INL64 = 0;
for i = 1:988
    INL = abs(y64(i)-ideal64(i));
    if(INL>INL64)
        INL64 = INL;
    end
end

INL32 = 0;
for i = 1:988
    INL = abs(y32(i)-ideal32(i));
    if(INL>INL32)
        INL32 = INL;
    end
end

INL16 = 0;
for i = 1:988
    INL = abs(y16(i)-ideal16(i));
    if(INL>INL16)
        INL16 = INL;
    end
end

INL8 = 0;
for i = 1:988
    INL = abs(y8(i)-ideal8(i));
    if(INL>INL8)
        INL8 = INL;
    end
end

INL4 = 0;
for i = 1:988
    INL = abs(y4(i)-ideal4(i));
    if(INL>INL4)
        INL4 = INL;
    end
end

INL2 = 0;
for i = 1:988
    INL = abs(y2(i)-ideal2(i));
    if(INL>INL2)
        INL2 = INL;
    end
end

%plot INL
figure(3);
INLarray = [INL2, INL4, INL8, INL16, INL32, INL64, INL128];
prescale=[2,4,8,16,32,64,128];
semilogx(prescale,INLarray);
xlim([2 128]);
xticks([2 4 8 16 32 64 128]);
xlabel("ADC Prescale Value");
ylabel("Integral Non-Linearity (V)");
title("Relationship Between Prescale Value and Integral non-Linearity");

