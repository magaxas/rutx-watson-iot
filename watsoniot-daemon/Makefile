include $(TOPDIR)/rules.mk

PKG_NAME:=watsoniot-daemon
PKG_RELEASE:=1
PKG_VERSION:=1.0.0

include $(INCLUDE_DIR)/package.mk

define Package/watsoniot-daemon
	CATEGORY:=Base system
	TITLE:=watsoniot-daemon
	DEPENDS:=+luci-app-watsoniot +libwatsoniot +libubox +libubus +libblobmsg-json
endef

define Package/watsoniot-daemon/description
	Daemon to send router data to the IBM IoT platform
endef

define Package/watsoniot-daemon/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/watsoniot-daemon $(1)/usr/bin
	$(INSTALL_BIN) ./files/watsoniot-daemon.init $(1)/etc/init.d/watsoniot-daemon
	$(INSTALL_CONF) ./files/watsoniot-daemon.config $(1)/etc/config/watsoniot-daemon
endef

$(eval $(call BuildPackage,watsoniot-daemon))
