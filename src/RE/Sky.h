#pragma once


namespace RE
{
	class Atmosphere;
	class BGSSaveGameBuffer;
	class Clouds;
	class Moon;
	class Precipitation;
	class ReferenceEffect;
	class SkySound;
	class Stars;
	class Sun;

	class ReferenceEffectController
	{
	public:
		static constexpr auto RTTI{ RTTI::ReferenceEffectController };
		static constexpr auto VTABLE{ VTABLE::ReferenceEffectController };

		virtual ~ReferenceEffectController();  // 00

		// add
		virtual void HandleEvent(const BSFixedString& a_event);  // 01
		virtual float GetElapsedTime();                          // 02
		virtual float GetScale();                                // 03
		virtual void SwitchAttachedRoot(NiNode*, NiNode*);       // 04
		virtual const NiPoint3* GetSourcePosition();             // 05
		virtual bool GetUseSourcePosition();                     // 06
		virtual bool GetNoInitialFlare();                        // 07
		virtual bool GetEffectPersists();                        // 08
		virtual bool GetGoryVisuals();                           // 09
		virtual void RemoveHitEffect(ReferenceEffect*) = 0;      // 10
		virtual TESObjectREFR* GetTargetReference() = 0;         // 11
		virtual BGSArtObject* GetHitEffectArt() = 0;             // 12
		virtual TESEffectShader* GetHitEffectShader() = 0;       // 13
		virtual bool GetManagerHandlesSaveLoad();                // 14
		virtual NiAVObject* GetAttachRoot();                     // 15
		virtual float GetParticleAttachExtent();                 // 16
		virtual bool GetUseParticleAttachExtent();               // 17
		virtual bool GetDoParticles();                           // 18
		virtual bool GetParticlesUseLocalSpace();                // 19
		virtual bool GetUseRootWorldRotate();                    // 20
		virtual bool GetIsRootActor();                           // 21
		virtual bool GetClearWhenCellIsUnloaded();               // 22
		virtual bool EffectShouldFaceTarget();                   // 23
		virtual TESObjectREFR* GetFacingTarget();                // 24
		virtual bool GetShaderUseParentCell();                   // 25
		virtual bool EffectAttachesToCamera();                   // 26
		virtual bool EffectRotatesWithCamera();                  // 27
		virtual bool GetAllowTargetRoot();                       // 28
		virtual bool IsReadyForAttach();                         // 29
		virtual void SetWindPoint(const NiPoint3& a_windPoint);  // 30
		virtual const NiPoint3* GetWindPoint();                  // 31
		virtual bool GetAllowNo3D();                             // 32
		virtual void SaveGame(BGSSaveGameBuffer* a_buffer);      // 33
		virtual void LoadGame(BGSLoadGameBuffer* a_buffer);      // 34
	};
	static_assert(sizeof(ReferenceEffectController) == 0x08);

	class SkyEffectController :
		public ReferenceEffectController
	{
	public:
		static constexpr auto RTTI{ RTTI::SkyEffectController };
		static constexpr auto VTABLE{ VTABLE::SkyEffectController };

		// override (ReferenceEffectController)
		virtual void RemoveHitEffect(ReferenceEffect*) override;  // 10
		virtual TESObjectREFR* GetTargetReference() override;     // 11
		virtual BGSArtObject* GetHitEffectArt() override;         // 12
		virtual TESEffectShader* GetHitEffectShader() override;   // 13
		virtual bool GetManagerHandlesSaveLoad() override;        // 14
		virtual NiAVObject* GetAttachRoot() override;             // 15
		virtual bool EffectAttachesToCamera() override;           // 26
		virtual bool EffectRotatesWithCamera() override;          // 27
	};
	static_assert(sizeof(SkyEffectController) == 0x08);

	class Sky
	{
	public:
		static constexpr auto RTTI{ RTTI::Sky };
		static constexpr auto VTABLE{ VTABLE::Sky };

		enum SKY_MODE : std::int32_t
		{
			kNone = 0,
			kInterior = 1,
			kSkyDomeOnly = 2,
			kFull = 3
		};

		struct SkyStaticRefData
		{
		public:
			// members
			BSTTuple<ObjectRefHandle, std::uint8_t> data;  // 00
		};
		static_assert(sizeof(SkyStaticRefData) == 0x08);

		virtual ~Sky();  // 00

		static Sky* GetSingleton()
		{
			REL::Relocation<Sky**> singleton{ REL::ID(1445899) };
			return *singleton;
		}

		// members
		NiPointer<BSMultiBoundNode> root;                              // 008
		NiPointer<NiNode> moonsRoot;                                   // 010
		NiPointer<NiNode> auroraRoot;                                  // 018
		void* aurora3D;                                                // 020
		BGSLightingTemplate* extLightingOverride;                      // 028
		ObjectRefHandle currentRoom;                                   // 030
		ObjectRefHandle previousRoom;                                  // 034
		float lightingTransition;                                      // 038
		float lightingTransitionTimer;                                 // 03C
		TESClimate* currentClimate;                                    // 040
		TESWeather* currentWeather;                                    // 048
		TESWeather* lastWeather;                                       // 050
		TESWeather* defaultWeather;                                    // 058
		TESWeather* overrideWeather;                                   // 060
		TESRegion* currentRegion;                                      // 058
		Atmosphere* atmosphere;                                        // 070
		Stars* stars;                                                  // 078
		Sun* sun;                                                      // 080
		Clouds* clouds;                                                // 088
		Moon* masser;                                                  // 090
		Moon* secunda;                                                 // 098
		Precipitation* precipitation;                                  // 0A0
		NiColor skyColor[19];                                          // 0A8
		NiColor prevSkyColor[19];                                      // 18C
		NiColor prevDirAmbColors[3][2];                                // 270
		NiColorA prevSpecTintFres;                                     // 2B8
		TESImageSpace* prevImageSpace;                                 // 2C8
		float prevFogDistances[8];                                     // 2D0
		float prevFogHeight;                                           // 2F0
		float prevFogPower;                                            // 2F4
		float prevFogClamp;                                            // 2F8
		float prevFogHighDensityScale;                                 // 2FC
		float lastExtWetness;                                          // 300
		NiColor waterFogColor;                                         // 304
		NiColor sunSpecularColor;                                      // 310
		float windSpeed;                                               // 31C
		float windAngle;                                               // 320
		float windTurbulence;                                          // 324
		float fogDistances[8];                                         // 328
		float fogHeight;                                               // 348
		float fogPower;                                                // 34C
		float fogClamp;                                                // 350
		float fogHighDensityScale;                                     // 354
		float currentGameHour;                                         // 358
		float lastWeatherUpdate;                                       // 35C
		float currentWeatherPct;                                       // 360
		float lastWindDirection;                                       // 364
		float lastWindDirectionRange;                                  // 368
		stl::enumeration<SKY_MODE, std::int32_t> mode;                 // 36C
		BSSimpleList<SkySound*>* skySoundList;                         // 370
		float flash;                                                   // 378
		std::uint64_t flashTime;                                       // 380
		std::uint32_t lastMoonPhaseUpdate;                             // 388
		float windowReflectionTimer;                                   // 38C
		float accelBeginPercent;                                       // 390
		std::uint32_t flags;                                           // 394
		ImageSpaceModifierInstanceForm* currentWeatherImageSpaceMod;   // 398
		ImageSpaceModifierInstanceForm* currentWeatherImageSpaceMod2;  // 3A0
		ImageSpaceModifierInstanceForm* lastWeatherImageSpaceMod;      // 3A8
		ImageSpaceModifierInstanceForm* lastWeatherImageSpaceMod2;     // 3B0
		NiColor directionalAmbientColors[3][2];                        // 3B8
		NiColor ambientSpecularTint;                                   // 400
		float ambientSpecularFresnel;                                  // 40C
		float auroraInStart;                                           // 410
		float auroraIn;                                                // 414
		float auroraOutStart;                                          // 418
		float auroraOut;                                               // 41C
		NiPointer<ReferenceEffect> currentReferenceEffect;             // 420
		NiPointer<ReferenceEffect> lastReferenceEffect;                // 428
		SkyEffectController effectController;                          // 430
		BSTArray<NiPointer<NiTexture>> storedCloudTextures;            // 438
		BSTArray<NiPointer<NiTexture>> storedWorldMapCloudTextures;    // 450
		BSTArray<SkyStaticRefData> skyStaticRefData;                   // 468
	};
	static_assert(sizeof(Sky) == 0x480);
}
