#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofEnableAlphaBlending();
    ofBackground(255);
    ofSetVerticalSync(true);
  
    
    leap.open();
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
    for(int i = 0; i < 7; i++){
        //初期位置を画面の中心に
        pos[i].x = ofGetWidth()/2;
        pos[i].y = ofGetHeight()/2;
    
        //速度をランダムに決定
        vel[i].x = ofRandom(1,5);
        vel[i].y = ofRandom(1,5);
    }
    
    //音
     sound[0].loadSound("do.mp3");
     sound[1].loadSound("re.mp3");
     sound[2].loadSound("mi.mp3");
     sound[3].loadSound("fa.mp3");
     sound[4].loadSound("so.mp3");
     sound[5].loadSound("ra.mp3");
     sound[6].loadSound("si.mp3");
   
    //ofMeshで背景の色を描画
    
    int wid = ofGetWidth()/7;
    int hei = ofGetHeight();
   
    //メモ：colorのSaturationを変数にいれてしまう
    
    for(int i = 0; i < 7; i++){
       
            
                backMesh.addVertex(ofVec3f(wid*i, 0, 1));//左下
                backMesh.addColor(ofColor::fromHsb(color[i], 0, 255));
                
                backMesh.addVertex(ofVec3f(wid*(i+1), 0, 1));//右下
                backMesh.addColor(ofColor::fromHsb(color[i], 0, 255));
                
                backMesh.addVertex(ofVec3f(wid*i, hei, 1));//左上
                backMesh.addColor(ofColor::fromHsb(color[i], 255, 255));
                
                backMesh.addVertex(ofVec3f(wid*i, hei, 1));//左上
                backMesh.addColor(ofColor::fromHsb(color[i], 255, 255));
                
                backMesh.addVertex(ofVec3f(wid*(i+1), hei, 1));//右上
                backMesh.addColor(ofColor::fromHsb(color[i], 255, 255));
                
                backMesh.addVertex(ofVec3f(wid*(i+1), 0, 1));//右下
                backMesh.addColor(ofColor::fromHsb(color[i], 0, 255));
        
    }
    
   
   
}

//--------------------------------------------------------------
void ofApp::update(){

    //---LeapMotion---
    simpleHands = leap.getSimpleHands();//simpleHandsはベクター
    if( leap.isFrameNew() && simpleHands.size() ){//LeapMotionのフレームが更新されるかつ、ひとつ以上の手が検出されたとき
        fingerPos.clear();
        handPos.clear();
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
        leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);

        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            handPos.push_back(simpleHands[i].handPos);
            for (int f=0; f<5; f++) {
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;
                fingerPos.push_back(mcp);// metacarpal
                
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;
                fingerPos.push_back(pip);// proximal
                
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;
                fingerPos.push_back(dip); // distal
                
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;
                fingerPos.push_back(tip);// fingertip
                
            }
        }
    }
    leap.markFrameAsOld();

    //---壁での反射---
    for(int i = 0; i < 7; i++){
        pos[i] += vel[i];
        
        if(pos[i].x < 50 ){ //左
            vel[i].x *= -1;
        }
        if(ofGetWidth()-50 < pos[i].x){ //右
            vel[i].x *= -1;
        }
        
        if(pos[i].y < 50 ){ //下
            vel[i].y *= -1;
        }
        
        if(ofGetHeight()-50 < pos[i].y){ //上
            vel[i].y *= -1;
        }
        
        
        //---手に当たった反射・色の変化---
        if(((x_circle > (pos[i].x - ofGetWidth()/2)-50) && (x_circle < (pos[i].x - ofGetWidth()/2)+50)) && ((y_circle > (pos[i].y-ofGetHeight()/2)-50) && (y_circle < (pos[i].y-ofGetHeight()/2)+50))){
            
            vel[i] *= -1;
            sound[i].play();
            
            //---balleffect---
            volume = ofSoundGetSpectrum(1);
            size_circle = volume[0]*3000;
        }
    }
    

    
}

//--------------------------------------------------------------
void ofApp::draw(){
    cam.begin();
    
    ofPushMatrix();
    ofRotateX(180);
    ofTranslate(-ofGetWidth()/2, -ofGetHeight()/2);
    ofSetColor(180);
   
        
    switch (stage_mode) {
        case 0:
            backMesh.draw();
            break;
        case 1:
            break;
        default:
            break;
    }

   
    ofPopMatrix();
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        hand[i]=handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        //手の中心に円を描画
        ofSetColor(200);
        ofDrawCircle(handPos.x, handPos.y, 20);
        
        for (int f=0; f<5; f++) {//手の関節を描画するためのfor文
            ofDrawCircle(fingerPos.at(20*i+4*f+0), 10); //metacarpal
            ofDrawCircle(fingerPos.at(20*i+4*f+1), 10);   // proximal
            ofDrawCircle(fingerPos.at(20*i+4*f+2), 10);   // distal
            ofDrawCircle(fingerPos.at(20*i+4*f+3), 10);   // fingertip
            
            //手の骨を描画
            ofSetLineWidth(10);
            ofDrawLine(fingerPos.at(20*i+4*f+0),fingerPos.at(20*i+4*f+1));
            ofDrawLine(fingerPos.at(20*i+4*f+1),fingerPos.at(20*i+4*f+2));
            ofDrawLine(fingerPos.at(20*i+4*f+2),fingerPos.at(20*i+4*f+3));
        }//手の関節を描画するためのfor文
       
        x_circle = handPos.x;
        y_circle = handPos.y;
        //  cout << x_circle << endl;
    }//手の数だけfor文を回す
    
    
    for(int i = 0; i < 7; i++){//動いている円
        ofSetColor(ofColor::fromHsb(color[i], 255, 255, 200));
        ofFill();
        ofDrawCircle(pos[i].x - ofGetWidth()/2, pos[i].y-ofGetHeight()/2,50); //camの中でずれている位置を調整
       
        if(((x_circle > (pos[i].x - ofGetWidth()/2)-50) && (x_circle < (pos[i].x - ofGetWidth()/2)+50)) && ((y_circle > (pos[i].y-ofGetHeight()/2)-50) && (y_circle < (pos[i].y-ofGetHeight()/2)+50))){
                ofNoFill();
                ofSetLineWidth(3);
                ofDrawCircle(pos[i].x - ofGetWidth()/2, pos[i].y-ofGetHeight()/2, 50+size_circle);
            //cout << size_circle << endl;
            
        }
    }
    
    
    cam.end();
    

    
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if (key == ' ') {
        stage_mode=(stage_mode+1)%2;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
    x_circle = x;
    y_circle = y;
    
    }

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
