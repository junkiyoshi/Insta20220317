#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	ofSetFrameRate(30);
	ofSetWindowTitle("openframeworks");

	ofBackground(255);
	ofSetColor(0);

	ofTrueTypeFont font;
	font.loadFont("fonts/Kazesawa-Bold.ttf", 100, true, true, true);

	for (char c = 'A'; c <= 'Z'; c++) {

		this->chara_path_list.push_back(font.getCharacterAsPoints(c, true, false));
	}

	for (int x = -300; x <= 300; x += 15) {

		for (int y = -300; y <= 300; y += 15) {

			this->location_list.push_back(glm::vec2(x, y));
			this->life_list.push_back(0);
			this->noise_param_list.push_back(ofRandom(1000));

		}
	}
}

//--------------------------------------------------------------
void ofApp::update() {

	ofSeedRandom(39);

	this->noise_location_list.clear();

	for (int i = 0; i < this->location_list.size(); i++) {

		this->life_list[i] = this->life_list[i] > 0 ? this->life_list[i] - 0.5 : 0;
		
		if (this->life_list[i] > 20) {

		}
		else if (this->life_list[i] > 0) {

			this->noise_param_list[i] += ofMap(this->life_list[i], 0, 20, 0.005, 0.1);
		}
	}

	for (int i = 0; i < 1; i++) {

		int deg_start = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.005), 0, 1, -360, 360);
		int radius = ofMap(ofNoise(ofRandom(1000), ofGetFrameNum() * 0.01), 0, 1, 0, 400);

		for (int deg = deg_start; deg < deg_start + 360; deg += 120) {

			auto noise_location = glm::vec2(radius * cos(deg * DEG_TO_RAD), radius * sin(deg * DEG_TO_RAD));
			this->noise_location_list.push_back(noise_location);
			for (int i = 0; i < this->location_list.size(); i++) {

				if (glm::distance(noise_location, this->location_list[i]) < 50) {

					this->life_list[i] = this->life_list[i] < 100 ? this->life_list[i] + 8 : 100;
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {

	ofTranslate(ofGetWindowSize() * 0.5);

	// 配色デザイン ビビッドレッド P038
	vector<ofColor> color_palette;
	color_palette.push_back(ofColor(197, 0, 24));
	color_palette.push_back(ofColor(184, 12, 65));
	color_palette.push_back(ofColor(206, 97, 110));
	color_palette.push_back(ofColor(204, 85, 68));
	color_palette.push_back(ofColor(190, 145, 176));
	color_palette.push_back(ofColor(215, 130, 63));
	color_palette.push_back(ofColor(255, 241, 51));
	color_palette.push_back(ofColor(107, 182, 187));

	for (int i = 0; i < this->location_list.size(); i++) {

		auto color = color_palette[ofRandom(color_palette.size())];
		auto fill_flag = ofRandom(1) < 0.25;

		if (this->life_list[i] > 20) {

		}
		else if (this->life_list[i] > 0) {

			ofSetColor(color);

			ofPushMatrix();
			ofTranslate(this->location_list[i]);

			int chara_index = ofMap(ofNoise(this->noise_param_list[i]), 0, 1, 0, this->chara_path_list.size());
			auto outline = this->chara_path_list[chara_index].getOutline();
			fill_flag < 0.5 ? ofNoFill() : ofFill();
			auto font_size = ofMap(this->life_list[i], 0, 20, 13, 0);

			ofBeginShape();
			for (int line_index = 0; line_index < outline.size(); line_index++) {

				if (line_index != 0) { ofNextContour(true); }

				auto vertices = outline[line_index].getVertices();
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto point = (vertices[vertices_index] / 100 * font_size) + glm::vec2(font_size * -0.5, font_size * 0.5);
					ofVertex(point);
				}
			}
			ofEndShape(true);

			ofPopMatrix();
		}
		else {

			ofSetColor(color);

			ofPushMatrix();
			ofTranslate(this->location_list[i]);

			int chara_index = ofMap(ofNoise(this->noise_param_list[i]), 0, 1, 0, this->chara_path_list.size());
			auto outline = this->chara_path_list[chara_index].getOutline();
			fill_flag ? ofNoFill() : ofFill();
			float font_size = 13;

			ofBeginShape();
			for (int line_index = 0; line_index < outline.size(); line_index++) {

				if (line_index != 0) { ofNextContour(true); }

				auto vertices = outline[line_index].getVertices();
				for (int vertices_index = 0; vertices_index < vertices.size(); vertices_index++) {

					auto point = (vertices[vertices_index] / 100 * font_size) + glm::vec2(font_size * -0.5, font_size * 0.5);
					ofVertex(point);
				}
			}
			ofEndShape(true);

			ofPopMatrix();;
		}
	}

	ofSetColor(color_palette[0]);
	for (int i = 0; i < this->noise_location_list.size(); i++) {

		ofDrawCircle(this->noise_location_list[i], 15);
	}
}

//--------------------------------------------------------------
int main() {

	ofSetupOpenGL(720, 720, OF_WINDOW);
	ofRunApp(new ofApp());
}