include $(TOPDIR)/rules.mk

PKG_NAME:=libwatsoniot
PKG_RELEASE:=1
PKG_VERSION:=1.0.1
PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/ibm-watson-iot/iot-c.git
PKG_SOURCE_VERSION:=c979fa28484af120b791f8c28537aa74a9077a91

include $(INCLUDE_DIR)/package.mk

define Package/libwatsoniot
	CATEGORY:=Libraries
	TITLE:=libwatsoniot
	DEPENDS:=+libopenssl
endef

define Package/libwatsoniot/description
	Library for IBM watson IoT platform integration
endef

define Package/libwatsoniot/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so* $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so* $(1)/usr/lib
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/lib $(1)/usr/include
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/build/libiotp-as-device.so* $(1)/usr/lib
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/paho.mqtt.c/build/output/libpaho-mqtt3as.so* $(1)/usr/lib
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/src/wiotp/sdk/*.h $(1)/usr/include/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/paho.mqtt.c/src/*.h $(1)/usr/include/
endef

$(eval $(call BuildPackage,libwatsoniot))
