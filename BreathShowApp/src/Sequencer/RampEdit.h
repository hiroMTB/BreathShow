//
//  RampEdit.h
//  ImSequenceTest
//
//  Created by Hiroshi Matoba on 17.09.22.
//

#ifndef RampEdit_h
#define RampEdit_h

#include "ImCurveEdit.h"

class RampEdit : public ImCurveEdit::Delegate
{

public:
	RampEdit()
    {
        mPts.clear();
        //makeDefault();
        mbVisible[0] = mbVisible[1] = mbVisible[2] = mbVisible[3] = true;
        mMax = ImVec2(1.0, 1.0);
        mMin = ImVec2(0.0, 0.0);
    }
    
    void makeDefault(){
//        mPts.clear();
//        mPts.assign(4, std::vector<ImVec2>());
//
//        mPts[0].emplace_back( 0.f, 0.5f );
//        mPts[0].emplace_back( 10000.f, 0.7f );
//        mPts[1].emplace_back( 0.f, 0.5f );
//        mPts[1].emplace_back( 10000.f, 0.5f );
//        mPts[2].emplace_back( 0.f, 0.5f );
//        mPts[2].emplace_back( 10000.f, 0.1f );
//        mPts[3].emplace_back( 0.f, 0.5f );
//		mPts[3].emplace_back( 10000.f, 0.5f );
//
//		mbVisible[0] = mbVisible[1] = mbVisible[2] = mbVisible[3] = true;
//
	}

    void sortAll(){
        SortValues(0);
        SortValues(1);
        SortValues(2);
        SortValues(3);
    }
    
	size_t GetCurveCount() override
	{
		return mPts.size();
	}

	bool IsVisible(size_t curveIndex) override
	{
		return mbVisible[curveIndex];
	}

	size_t GetPointCount(size_t curveIndex) override
	{
		return mPts[curveIndex].size();
	}

	uint32_t GetCurveColor(size_t curveIndex) override
	{
		uint32_t cols[] = {0xFF0000FF, 0xFF00FF00, 0xFFFF0000, 0xFF999999};
		return cols[curveIndex];
	}

	std::vector<ImVec2> &GetPoints(size_t curveIndex) override
	{
		return mPts[curveIndex];
	}

	virtual ImCurveEdit::CurveType GetCurveType(size_t curveIndex) const override
	{
		return ImCurveEdit::CurveSmooth;
	}

	virtual int EditPoint(size_t curveIndex, int pointIndex, const ImVec2 &value) override
	{
		mPts[curveIndex][pointIndex] = ImVec2(value.x, value.y);
		SortValues(curveIndex);
		for (size_t i = 0; i < GetPointCount(curveIndex); i++)
		{
			if (mPts[curveIndex][i].x == value.x)
				return (int)i;
		}
		return pointIndex;
	}

	virtual void AddPoint(size_t curveIndex, const ImVec2 &value) override
	{
		mPts[curveIndex].emplace_back(value);
		SortValues(curveIndex);
	}

	virtual ImVec2 &GetMax() override { return mMax; }
	virtual ImVec2 &GetMin() override { return mMin; }
	virtual unsigned int GetBackgroundColor() override { return 0; }

	std::vector<std::vector<ImVec2>> mPts;
	bool mbVisible[4];
	ImVec2 mMin;
	ImVec2 mMax;

private:
	void SortValues(size_t curveIndex)
	{
		auto b = mPts[curveIndex].begin();
		auto e = mPts[curveIndex].end();
		std::sort(b, e, [](ImVec2 a, ImVec2 b)
					 { return a.x < b.x; });
	}
};

#endif /* RampEdit_h */
