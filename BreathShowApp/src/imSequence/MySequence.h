//
//  MySequence.h
//

#ifndef MySequence_h
#define MySequence_h

//#include "imgui.h"
#include "ImSequencer.h"
#include "imgui_internal.h"
#include "RampEdit.h"
#include "ImCurveEdit.h"

static const char *SequencerItemTypeNames[] = {"Camera", "Music", "ScreenEffect", "FadeIn", "Animation"};
class MySequence : public ImSequencer::SequenceInterface{
   
public:
   // interface with sequencer

   virtual int GetFrameMin() const
   {
      return mFrameMin;
   }
   virtual int GetFrameMax() const
   {
      return mFrameMax;
   }
   virtual int GetItemCount() const { return (int)myItems.size(); }

   virtual int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char *); }
   virtual const char *GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
   virtual const char *GetItemLabel(int index) const
   {
      static char tmps[512];
      snprintf(tmps, 512, "[%02d] %s", index, SequencerItemTypeNames[myItems[index].mType]);
      return tmps;
   }

   virtual void Get(int index, int **start, int **end, int *type, unsigned int *color)
   {
      MySequenceItem &item = myItems[index];
      if (color)
         *color = 0xFFAA8080; // same color for everyone, return color based on type
      if (start)
         *start = &item.mFrameStart;
      if (end)
         *end = &item.mFrameEnd;
      if (type)
         *type = item.mType;
   }
   virtual void Add(int type) { myItems.push_back(MySequenceItem{type, 0, 10, false}); };
   virtual void Del(int index) { myItems.erase(myItems.begin() + index); }
   virtual void Duplicate(int index) { myItems.push_back(myItems[index]); }

   virtual size_t GetCustomHeight(int index) { return myItems[index].mExpanded ? 100 : 0; }

   // my datas
   MySequence() : mFrameMin(0), mFrameMax(0) {}
   int mFrameMin, mFrameMax;
   struct MySequenceItem
   {
      int mType;
      int mFrameStart, mFrameEnd;
      bool mExpanded;
      RampEdit rampEdit;
   };
   std::vector<MySequenceItem> myItems;

   virtual void DoubleClick(int index)
   {
      if (myItems[index].mExpanded)
      {
         myItems[index].mExpanded = false;
         return;
      }
       for (auto& item : myItems)
          item.mExpanded = false;
      myItems[index].mExpanded = !myItems[index].mExpanded;
   }

   virtual void CustomDraw(int index, ImDrawList *draw_list, const ImRect &rc, const ImRect &legendRect, const ImRect &clippingRect, const ImRect &legendClippingRect)
   {
      static const char *labels[] = {"Translation", "Rotation", "Scale"};
      RampEdit & rampEdit = myItems[index].rampEdit;
       
      rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
      rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
      draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);
      for (int i = 0; i < 3; i++)
      {
         ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
         ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
         draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);
         if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
            rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
      }
      draw_list->PopClipRect();

      ImGui::SetCursorScreenPos(rc.Min);
      ImVec2 diff;
      diff.x = rc.Max.x - rc.Min.x;
      diff.y = rc.Max.y - rc.Min.y;
      ImCurveEdit::Edit(rampEdit, diff, 137 + index, &clippingRect);
   }

   virtual void CustomDrawCompact(int index, ImDrawList *draw_list, const ImRect &rc, const ImRect &clippingRect)
   {
      RampEdit & rampEdit = myItems[index].rampEdit;
       
      rampEdit.mMax = ImVec2(float(mFrameMax), 1.f);
      rampEdit.mMin = ImVec2(float(mFrameMin), 0.f);
      draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);
      for (int i = 0; i < 3; i++)
      {
         for (int j = 0; j < rampEdit.mPts.size(); j++)
         {
            float p = rampEdit.mPts[i][j].x;
            if (p < myItems[index].mFrameStart || p > myItems[index].mFrameEnd)
               continue;
            float r = (p - mFrameMin) / float(mFrameMax - mFrameMin);
            float x = ImLerp(rc.Min.x, rc.Max.x, r);
            draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
         }
      }
      draw_list->PopClipRect();
   }
    
    
};

#endif /* MySequence_h */
