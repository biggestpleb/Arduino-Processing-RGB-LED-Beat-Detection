void loadSettings(){
  settings = loadStrings("settings.txt");
  hue = getValue( settings[0] );
  saturation = getValue( settings[1] );
  intensity = getValue( settings[2] );
  hueDrift = getValue( settings[3] );
  barMultiplier = getValue( settings[4] );
  
  switchRate = getValue( settings[5] );
  hOffsetMax = getValue( settings[6] );
  hDecay = getValue( settings[7] );
  sOffsetMax = getValue( settings[8] );
  sDecay = getValue( settings[9] );
  
  iOffsetMax = getValue( settings[10] );  
  iDecay = getValue( settings[11] );
  iMax = getValue( settings[12] );  
  sMax = getValue( settings[13] );
  
  hOffsetMaxTemp = hOffsetMax;
  sOffsetMaxTemp = sOffsetMax;
  iOffsetMaxTemp = iOffsetMax;
  
  //saveSettings();
}

void saveSettings(){
  saveData = new String[14];
  saveData[0] = "hue: " + nf( int(hue), 3 ) ;
  saveData[1] = "saturation: " + nf(saturation, 1, 2) ;
  saveData[2] = ("intensity: " + nf(intensity, 1, 2) );
  saveData[3] = ("hueDrift: " + nf( int(hueDrift), 3) );
  saveData[4] = ("barHeight: " + nf(barMultiplier, 1, 2) );
  
  saveData[5] = ("switchRate: " + nf(switchRate, 1, 2) );
  saveData[6] = ("hOffsetMax: " + nf(hOffsetMax, 1, 2) );
  saveData[7] = ("hDecay: " + nf(hDecay, 1, 3) );
  saveData[8] = ("sOffsetMax: " + nf(sOffsetMax, 1, 2) );
  saveData[9] = ("sDecay: " + nf(sDecay, 1, 3) );
  
  saveData[10] = ("iOffsetMax: " + nf(iOffsetMax, 1, 2) );
  saveData[11] = ("iDecay: " + nf(iDecay, 1, 3) );  
  saveData[12] = ("iMax: " + nf(iMax, 1, 2) );
  saveData[13] = ("sMax: " + nf(iMax, 1, 3) );  
  //String something = "hello world";
  //saveData = split(something, ' ');/*
  println(saveData);
  saveStrings( "data/settings.txt", saveData);
}