
String returnDigits(int digits, int prefixSize=1){
  String x;
  // utility function for digital clock display: prints colon and leading 0
  
  if(digits < 10){
    x += "0";
  }
    
  if (prefixSize == 2){
     if(digits < 100){
      x += "0";
    }
  }
  if (prefixSize == 3){
     if(digits < 1000){
      x += "0";
     }
  }
  x+=digits;
  return x;
}


