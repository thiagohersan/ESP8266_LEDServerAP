const char *mColors[] = { "000000", "ff0000", "00ff00", "0000ff", "ff00ff", "ffff00", "00ffff", "ffffff"};
uint8_t nColors = sizeof(mColors) / sizeof(mColors[0]);

String createHtml() {
  String mHtml = "";
  mHtml += "<!DOCTYPE html><html><head><title>#VIBE#</title>";
  mHtml += "<meta name='viewport' content='width=device-width, initial-scale=1'>";
  mHtml += "<style> body{width:100vw;margin:0;padding:0;} input{appearance:none;} ";
  mHtml += ".b{width:100%;height:100%;border:4px #000 solid;} ";
  mHtml += ".c {width:100%;display:flex;flex-direction:row;flex-wrap:wrap;justify-content:center;} ";
  mHtml += ".f {width:23vw;height:23vw;padding:1vw;box-sizing:border-box;} "; 
  mHtml += "body{background:" + mColorHtml + ";} ";
  mHtml += ".sf {width:12vw;height:12vw;} </style></head>";

  mHtml += "<body><div class='c'>";
  for (uint8_t c = 0; c < nColors ; c++) {
    mHtml += "<form class='f' action='/' method='GET'><input type='hidden' name='color' value='";
    mHtml += mColors[c];
    mHtml += "'><input class='b' type='submit' value='' style='background:#";
    mHtml += mColors[c];
    mHtml += "'></form>";
  }

  for (uint8_t b = 0; b < 7 ; b++) {
    mHtml += "<form class='f sf' action='/' method='GET'><input type='hidden' name='brightness' value='";
    mHtml += String(b * 42);
    mHtml += "'><input class='b' type='submit' value='' style='background:rgb(";
    mHtml += String(b * 42) + "," + String(b * 42) + "," + String(b * 42);
    mHtml += ")'></form>";
  }

  mHtml += "</div></body></html>";
  return mHtml;
}
