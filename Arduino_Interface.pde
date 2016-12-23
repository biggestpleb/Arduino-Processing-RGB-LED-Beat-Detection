
//=========================================================================
//=======      L E D    D R I V E R    T H R E A D      ===================
//=========================================================================

//LED driver runs continuously and sends HSI values to the arduino
void ledDriver(){  
  
  String hsiData = "", prevData;
  
  while(true){
    delay(7); //this controls the transmit interval
    if(hue<0){
      hue += 360; 
    }
    hue = abs(hue)%360;
    
    //check for beats if beat detection enabled
    if(beatFlag && beatDetect){
      beatFlag = false;
      if(millis() - randomTime > 300){
        if(random(1) < switchRate){
          // ==== Generate random hue =====
          do{
            hue = (hue + random(60,240)) % 360; 
          } while ( abs((hue%120)-60) > 55 );
          
          if(random(-1,1)>0){
            hOffset = hOffsetMaxTemp; 
          } else {
            hOffset = -hOffsetMaxTemp; 
          }
        } else {
          // Case: beat detected but rng dictated no random colour
          if(random(-1,1)>0){
            hOffset = hOffsetMaxTemp; 
          } else {
            hOffset = -hOffsetMaxTemp; 
          }
        }
        //hue += 15;
        iSpikeVal = iOffsetMaxTemp;
        sSpikeVal = sOffsetMaxTemp;
        //if( random(-1,1)>1 ){
         // increment = 0.3; 
        //} else {
       //   increment = -0.3; 
       // }
        randomTime = millis();
      } else {
        //iSpikeVal = 1.5;
        //sSpikeVal = 1.5;
        //hSpikeVal = 20;
        
      }
    } else {
      beatFlag = false; 
    }
    
    //========== Handle any spikes ============
    if( hSpikeVal != 0 ){     
      if(millis()-hSpikeTime>300){
        hOffset = hSpikeVal; 
        hSpikeTime = millis();
      }
      hSpikeVal = 0;
    }
    if( sSpikeVal != 0 ){     
      if(millis()-sSpikeTime>300){
        sOffset = sSpikeVal; 
        sSpikeTime = millis();
      }
      sSpikeVal = 0;
    }
    if( iSpikeVal != 0 ){     
      if(millis()-iSpikeTime>300){
        iOffset = iSpikeVal; 
        iSpikeTime = millis();
      }
      iSpikeVal = 0;
    }
    //defualt hDecay = 0.99, s.92, i.9
   
    //======= Fade Offsets ==============
    hOffset = constrain(hOffset*hDecay,-180,180);
    sOffset = constrain(sOffset*sDecay, 0, 2);
    iOffset = constrain((iOffset)*iDecay, 0, 2);
    
    //memoryBlock = false;  //remove memory block
    //======== Drift Hue =========
    hue += hueDrift/(1000/7);
    
    sActual = constrain( saturation + sOffset, 0, sMax);
    iActual = constrain( intensity + iOffset, 0, iMax);
    boolean flag = false;
    if(hOffset > -(360 - 328) && hOffset < -(360 - 345) ){
      flag = true;
      println("sActual=" + sActual +"  iActual="+iActual + "  Hue:" + hue +"  hueOffset:" + hOffset);
    }
    
    prevData = hsiData; //log previous data
    //iActual = constrain( intensity + iOffset, 0, iMax);
    //======= Generate data string for Arduino ======
    hsiData = "h" + nf(fixHue(hue + hOffset), 3, 1);
    hsiData += "s" + nf(sActual, 1, 3);
    hsiData += "i" + nf(iActual, 1, 3);
    hsiData += "\n";
    //String hsiData = "h-023.2s0.32i0.25\n";
    if( portConnected ){
      if( hsiData.equals(prevData) == false ) arduino.write(hsiData);   //only send if there is new data
      println(hsiData);
    }
    if(flag) println("SENT: " + hsiData);
    //println((millis() - time) + " "+counter);
  } 
}

//constrain hue to interal 0->360
float fixHue( float input ){
  input %= 360;
  if(input<0) input += 360;
  return input;
}