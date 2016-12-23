void keyPressed(){
  //fill(0,1,1);
  //println( isCtrl + " " + key );
  if( isCtrl == true && (key == 's' || key == 'S') ){
    saveSettings();    
    //println(millis());
    saveMode = 1;
    saveTimer = millis() + 1500;
  } else if ( isCtrl == true && (key == 'l' || key == 'L')||(key == 'r' || key == 'R') ){
  
    loadSettings();
    saveMode = 2;
    saveTimer = millis() + 1500;
    
  }
  
  //saveTimer = millis() + 1500;
  if( key == CODED && keyCode == SHIFT ){
    //rect(100,100,150,150);
    isCtrl = true;
  } else {
    //isCtrl = false; 
  }
  
  //rect(1,1,50,50);
  
}

void keyReleased(){
  isCtrl = false; 
  //rect(210,210,500,500);
  
}

void mouseReleased(){
  currentSlider = 0; 
}