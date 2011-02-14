### Lists of source files, included by Makefiles

AROS_LIBDRM_CORE_SOURCES = \
            libdrm/arosdrm \
            libdrm/arosdrmmode \
            
AROS_LIBDRM_NVIDIA_SOURCES = \
            libdrm/nouveau/nouveau_device \
            libdrm/nouveau/nouveau_resource \
            libdrm/nouveau/nouveau_pushbuf \
            libdrm/nouveau/nouveau_notifier \
            libdrm/nouveau/nouveau_grobj \
            libdrm/nouveau/nouveau_channel \
            libdrm/nouveau/nouveau_bo \
            libdrm/nouveau/nouveau_reloc \
            libdrm/arosdrm_nouveau \

AROS_DRM_CORE_SOURCES = \
            drm-aros/drm_aros \
            drm-aros/drm_bufs \
            drm-aros/drm_pci \
            drm-aros/drm_drv \
            drm-aros/drm_compat_funcs \
            drm/drm_mm \
            drm/drm_irq \
            drm/drm_gem \
            drm/drm_cache \
            drm/drm_crtc \
            drm/drm_crtc_helper \
            drm/drm_edid \
            drm/drm_global \
            drm/drm_memory \
            drm/drm_modes \
            drm/drm_agpsupport \
            drm/ttm/ttm_bo \
            drm/ttm/ttm_bo_manager \
            drm/ttm/ttm_bo_util \
            drm/ttm/ttm_tt \
            drm/ttm/ttm_agp_backend \

AROS_DRM_NVIDIA_SOURCES = \
            drm-aros/nouveau/nouveau_drv \
            drm/nouveau/nouveau_bios \
            drm/nouveau/nouveau_bo_renamed \
            drm/nouveau/nouveau_channel_renamed \
            drm/nouveau/nouveau_calc \
            drm/nouveau/nouveau_connector \
            drm/nouveau/nouveau_display \
            drm/nouveau/nouveau_dma \
            drm/nouveau/nouveau_dp \
            drm/nouveau/nouveau_fence \
            drm/nouveau/nouveau_i2c \
            drm/nouveau/nouveau_irq \
            drm/nouveau/nouveau_gem \
            drm/nouveau/nouveau_hw \
            drm/nouveau/nouveau_mem \
            drm/nouveau/nouveau_notifier_renamed \
            drm/nouveau/nouveau_object \
            drm/nouveau/nouveau_sgdma \
            drm/nouveau/nouveau_state \
            drm/nouveau/nouveau_ramht \
            drm/nouveau/nouveau_temp \
            drm/nouveau/nouveau_ttm \
            drm/nouveau/nouveau_util \
            drm/nouveau/nouveau_volt \
            drm/nouveau/nv04_crtc \
            drm/nouveau/nv04_cursor \
            drm/nouveau/nv04_dac \
            drm/nouveau/nv04_dfp \
            drm/nouveau/nv04_display \
            drm/nouveau/nv04_fb \
            drm/nouveau/nv04_fifo \
            drm/nouveau/nv04_graph \
            drm/nouveau/nv04_instmem \
            drm/nouveau/nv04_mc \
            drm/nouveau/nv04_pm \
            drm/nouveau/nv04_timer \
            drm/nouveau/nv04_tv \
            drm/nouveau/nv10_fb \
            drm/nouveau/nv10_fifo \
            drm/nouveau/nv10_graph \
            drm/nouveau/nv10_gpio \
            drm/nouveau/nv17_tv \
            drm/nouveau/nv17_tv_modes \
            drm/nouveau/nv20_graph \
            drm/nouveau/nv30_fb \
            drm/nouveau/nv40_fb \
            drm/nouveau/nv40_fifo \
            drm/nouveau/nv40_graph \
            drm/nouveau/nv40_grctx \
            drm/nouveau/nv40_mc \
            drm/nouveau/nv50_instmem \
            drm/nouveau/nv50_display \
            drm/nouveau/nv50_calc \
            drm/nouveau/nv50_crtc \
            drm/nouveau/nv50_cursor \
            drm/nouveau/nv50_dac \
            drm/nouveau/nv50_evo \
            drm/nouveau/nv50_fb \
            drm/nouveau/nv50_fifo \
            drm/nouveau/nv50_mc \
            drm/nouveau/nv50_gpio \
            drm/nouveau/nv50_graph \
            drm/nouveau/nv50_grctx \
            drm/nouveau/nv50_pm \
            drm/nouveau/nv50_sor \
            drm/nouveau/nv84_crypt \
            drm/nouveau/nva3_pm \
            drm/nouveau/nvc0_fb \
            drm/nouveau/nvc0_fifo \
            drm/nouveau/nvc0_graph \
            drm/nouveau/nvc0_instmem \
