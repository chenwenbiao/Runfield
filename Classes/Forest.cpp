#include "Forest.h"

USING_NS_CC;

Forest::Forest(void) {
	_nForestVertices = 0;
}

Forest::~Forest(void) {
}

bool Forest::init(void) {
	bool bRet = false;
	do {
		CC_BREAK_IF(!CCNode::init());
		this->setShaderProgram(
				CCShaderCache::sharedShaderCache()->programForKey(
						kCCShader_PositionTexture));
		bRet = true;
	} while (0);
	return bRet;
}

void Forest::updateForest(float factor, float offset, float yoff, int maxH, int gap, int segW,
		Terrain* terrain) {
	_nForestVertices = 0;
	int screenH = CCDirector::sharedDirector()->getWinSize().height;
	int screenW = CCDirector::sharedDirector()->getWinSize().width;
	offset *= factor;
	float off = -(((int) floorf(offset)) % 512);
	const float h = maxH > 0 ? maxH : _forestTexture->getContentSize().height;
	const float w = _forestTexture->getContentSize().width;

	for (int i = 0; i < 1536 + 256; i += segW) {
		float x = off + i;
		if (x >= screenW) {
			continue;
		}
		float y = 512 - (floorf(terrain->forestY(factor, offset + off + i, gap) - yoff));
		float sw = MAX(0, MIN(w - (i % (int)w), segW));
		if (sw <= 0) {
			continue;
		}
		if (x + sw <= 0) {
			continue;
		}
		float sh = 0.f;
		if (y > 0) {
			if (y - h < screenH) {
				if (y < h) {
					sh = y;
				} else {
					sh = h;
				}
			} else {
				sh = 0;
				continue;
			}
		} else {
			sh = 0;
			continue;
		}

		_forestVertices[_nForestVertices] = ccp(x, y);
		_forestTexCoords[_nForestVertices++] = ccp(i / w, 0);
		_forestVertices[_nForestVertices] = ccp(x, y - sh);
		_forestTexCoords[_nForestVertices++] = ccp(i / w, sh / 512.f);
		_forestVertices[_nForestVertices] = ccp(x + sw, y -sh);
		_forestTexCoords[_nForestVertices++] = ccp((i + sw) / w, sh / 512.f);

		_forestVertices[_nForestVertices] = ccp(x + sw, y - sh);
		_forestTexCoords[_nForestVertices++] = ccp((i + sw) / w, sh / 512.f);
		_forestVertices[_nForestVertices] = ccp(x + sw, y);
		_forestTexCoords[_nForestVertices++] = ccp((i + sw) / w, 0);
		_forestVertices[_nForestVertices] = ccp(x, y);
		_forestTexCoords[_nForestVertices++] = ccp(i / w, 0);

		//		if (y + ih < 512) {
		//			for (var j = 0; j < 512 - y + ih; j += 32) {
		//				ctx.drawImage(img, (i % w), ih - 32, sw, 32, x, y + ih + j, sw,
		//						32);
		//			}
		//		}
	}
}

void Forest::draw() {
	CCNode::draw();

	CC_NODE_DRAW_SETUP();

	ccGLBindTexture2D(_forestTexture->getName());
	ccGLEnableVertexAttribs(
			kCCVertexAttribFlag_Position | kCCVertexAttribFlag_TexCoords);

	ccDrawColor4F(1.0f, 1.0f, 1.0f, 1.0f);
	glVertexAttribPointer(kCCVertexAttrib_Position, 2, GL_FLOAT, GL_FALSE, 0,
			_forestVertices);
	glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, 0,
			_forestTexCoords);
	glDrawArrays(GL_TRIANGLES, 0, (GLsizei) _nForestVertices);
}
