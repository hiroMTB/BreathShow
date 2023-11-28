#pragma once

#include "ofMain.h"

class Util {
    
public:
    static std::vector<std::string> split(std::string s, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> res;

        while ((pos_end = s.find(delimiter, pos_start)) != std::string::npos) {
            token = s.substr (pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            res.push_back (token);
        }

        res.push_back (s.substr (pos_start));
        return res;
    }

    static ofFbo::Settings getFboSettings(int w,
                                          int h,
                                          bool bUseDepth = false,
                                          GLint glFormat = GL_RGBA8,
                                          GLenum glTexTarget = GL_TEXTURE_2D)
    {
        ofFbo::Settings fboSettings;
        fboSettings.width = w;
        fboSettings.height = h;
        fboSettings.numSamples = 8;
        fboSettings.internalformat = glFormat;
        fboSettings.textureTarget = glTexTarget;
        fboSettings.useDepth = bUseDepth;
        return fboSettings;
    }

    /// @input val: -360 ~ 360
    /// @input min: -360 ~ 360
    /// @input max: -360 ~ 360
    static float ModularClamp(float val, float min, float max, float rangeMin=-180.0f, float rangeMax=180.0f){
        float modulus = std::abs(rangeMax - rangeMin); // 360

        if(min <= max){
            //if(val < rangeMin) val += modulus;
            //if(rangeMax < val) val -= modulus;
            return ofClamp(val, min, max);
        }else {
            if (fmodf(val, modulus) < 0.0f) val += modulus;
            if (fmodf(min, modulus) < 0.0f) min += modulus;
            if (fmodf(max, modulus) < 0.0f) max += modulus;

            // return value < min ? min : value > max ? max : value;
            float r = ofClamp(val, min, max);
            if (rangeMax < r) r -= modulus;
            if (r < rangeMin) r += modulus;
            return r;
        }
    }

    static void testModularClamp(float val, float min, float max, float expect){
        float r = Util::ModularClamp(val, min, max);
        cout << "val = " << val << ", min = " << min << ", max = " << max << "  --->  " << r << " : ";
        if(r == expect){
            cout << "OK" << endl;
        }else{
            cout << "XXX Wrong!" << endl;
        }
    }

    static void test(){
        cout << "Part 1" << endl;
        testModularClamp(0, -120, 120, 0);
        testModularClamp(60, -120, 120, 60);
        testModularClamp(-130, -120, 120, -120);
        testModularClamp(-190, -120, 120, -120);
        testModularClamp(-390, -120, 120, -120);
        testModularClamp(200, -120, 120, 120);

        cout << endl;
        cout << "Part 2" << endl;
        testModularClamp(1, 120, -120, 120);
        testModularClamp(60, 120, -120, 120);
        testModularClamp(-130, 120, -120, -130);
        testModularClamp(-190, 120, -120, 170);
        testModularClamp(-350, 120, -120, 120);
        testModularClamp(200, 120, -120, -160);
    }

};
