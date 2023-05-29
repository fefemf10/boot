export module glm.setup;

export namespace glm
{
	constexpr auto GLM_CLIP_CONTROL_ZO_BIT = (1 << 0); // ZERO_TO_ONE;
	constexpr auto GLM_CLIP_CONTROL_NO_BIT = (1 << 1); // NEGATIVE_ONE_TO_ONE
	constexpr auto GLM_CLIP_CONTROL_LH_BIT = (1 << 2); // LEFT_HANDED, For DirectX, Metal, Vulkan
	constexpr auto GLM_CLIP_CONTROL_RH_BIT = (1 << 3); // RIGHT_HANDED, For OpenGL, default in GLM;
	constexpr auto GLM_CLIP_CONTROL_LH_ZO = (GLM_CLIP_CONTROL_LH_BIT | GLM_CLIP_CONTROL_ZO_BIT);
	constexpr auto GLM_CLIP_CONTROL_LH_NO = (GLM_CLIP_CONTROL_LH_BIT | GLM_CLIP_CONTROL_NO_BIT);
	constexpr auto GLM_CLIP_CONTROL_RH_ZO = (GLM_CLIP_CONTROL_RH_BIT | GLM_CLIP_CONTROL_ZO_BIT);
	constexpr auto GLM_CLIP_CONTROL_RH_NO = (GLM_CLIP_CONTROL_RH_BIT | GLM_CLIP_CONTROL_NO_BIT);
	constexpr auto GLM_FORCE_DEPTH_ZERO_TO_ONE = 0;
	constexpr auto GLM_FORCE_LEFT_HANDED = 0;
	constexpr auto GLM_CONFIG_CLIP_CONTROL = GLM_CLIP_CONTROL_RH_NO;
}