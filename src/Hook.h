#pragma once

namespace SOB
{
	class LaunchArrowHook
	{
	public:
		static void Install()
		{
			REL::Relocation<uintptr_t> hook{ RELOCATION_ID(42928, 44108) };
			auto& trampoline = SKSE::GetTrampoline();
			SKSE::AllocTrampoline(1 << 4);
			_LaunchArrow = trampoline.write_call<5>(hook.address() + REL::Relocate(0x384, 0x38C), LaunchArrow);
		};

	private:
		static RE::ArrowProjectile* LaunchArrow(RE::ArrowProjectile* a_arrow, RE::Projectile::LaunchData* a_launchData)
		{
			using WEAP_FLAG = RE::TESObjectWEAP::Data::Flag;
			auto arrow = _LaunchArrow(a_arrow, a_launchData);
			auto weap = arrow ? arrow->weaponSource : nullptr;
			if (arrow && weap && (weap->IsBow() || weap->IsCrossbow()) && weap->weaponData.flags.any(WEAP_FLAG::kEmbeddedWeapon) && !weap->embeddedNode.empty()) {
				a_arrow->speedMult *= std ::stof(arrow->weaponSource->embeddedNode.c_str());
			}

			return arrow;
		}

		static inline REL::Relocation<decltype(LaunchArrow)> _LaunchArrow;
	};

}