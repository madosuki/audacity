/**********************************************************************

  Audacity: A Digital Audio Editor

  TruncSilence.h

  Lynn Allan (from DM's Normalize)
  //ToDo ... put BlendFrames in Effects, Project, or other class
  //ToDo ... Use ZeroCrossing logic to improve blend
  //ToDo ... BlendFrames on "fade-out"
  //ToDo ... BlendFrameCount is a user-selectable parameter
  //ToDo ... Detect transient signals that are too short to interrupt the TruncatableSilence
  Philip Van Baren (more options and boundary fixes)

**********************************************************************/

#ifndef __AUDACITY_EFFECT_TRUNC_SILENCE__
#define __AUDACITY_EFFECT_TRUNC_SILENCE__

#include "Effect.h"

class ShuttleGui;
class wxChoice;
class wxTextCtrl;
class wxCheckBox;

class RegionList;

class EffectTruncSilence final : public Effect
{
public:
   static const ComponentInterfaceSymbol Symbol;

   EffectTruncSilence();
   virtual ~EffectTruncSilence();

   // ComponentInterface implementation

   ComponentInterfaceSymbol GetSymbol() const override;
   TranslatableString GetDescription() const override;
   ManualPageID ManualPage() const override;

   // EffectDefinitionInterface implementation

   EffectType GetType() const override;
   bool GetAutomationParameters(CommandParameters & parms) override;
   bool SetAutomationParameters(CommandParameters & parms) override;

   // EffectProcessor implementation

   bool DefineParams( ShuttleParams & S ) override;

   // Effect implementation

   double CalcPreviewInputLength(
      const EffectSettings &settings, double previewLength) override;

   // Analyze a single track to find silences
   // If inputLength is not NULL we are calculating the minimum
   // amount of input for previewing.
   bool Analyze(RegionList &silenceList,
                        RegionList &trackSilences,
                        const WaveTrack *wt,
                        sampleCount* silentFrame,
                        sampleCount* index,
                        int whichTrack,
                        double* inputLength = NULL,
                        double* minInputLength = NULL);

   bool Process(EffectSettings &settings) override;
   std::unique_ptr<EffectUIValidator> PopulateOrExchange(
      ShuttleGui & S, EffectSettingsAccess &access) override;
   bool TransferDataToWindow(const EffectSettings &settings) override;
   bool TransferDataFromWindow(EffectSettings &settings) override;

private:
   // EffectTruncSilence implementation

   //ToDo ... put BlendFrames in Effects, Project, or other class
   // void BlendFrames(float* buffer, int leftIndex, int rightIndex, int blendFrameCount);
   void Intersect(RegionList &dest, const RegionList & src);

   void OnControlChange(wxCommandEvent & evt);
   void UpdateUI();

   bool ProcessIndependently();
   bool ProcessAll();
   bool FindSilences
      (RegionList &silences, const TrackList *list,
       const Track *firstTrack, const Track *lastTrack);
   bool DoRemoval
      (const RegionList &silences, unsigned iGroup, unsigned nGroups, Track *firstTrack, Track *lastTrack,
       double &totalCutLen);

private:

   double mThresholdDB {} ;
   int mActionIndex;
   double mInitialAllowedSilence;
   double mTruncLongestAllowedSilence;
   double mSilenceCompressPercent;
   bool mbIndependent;

   size_t mBlendFrameCount;

   wxTextCtrl *mThresholdText;
   wxChoice *mActionChoice;
   wxTextCtrl *mInitialAllowedSilenceT;
   wxTextCtrl *mTruncLongestAllowedSilenceT;
   wxTextCtrl *mSilenceCompressPercentT;
   wxCheckBox *mIndependent;

   DECLARE_EVENT_TABLE()
};

#endif
