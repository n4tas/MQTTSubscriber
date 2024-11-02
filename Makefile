include $(TOPDIR)/rules.mk

PKG_NAME:=MQTTSubscriber
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_BUILD_DEPENDS:=!USE_GLIBC:argp-standalone

include $(INCLUDE_DIR)/package.mk

define Package/MQTTSubscriber
	CATEGORY:=Base system
	TITLE:=MQTTSubscriber
	DEPENDS:=+libmosquitto +libsqlite3 +libuci +libcurl
endef

ifndef CONFIG_USE_GLIBC
	CONFIGURE_VARS += LIBS="-largp"
endif

define Package/MQTTSubscriber/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/MQTTSubscriber $(1)/usr/bin
	$(INSTALL_CONF) ./files/mqtt_login.config $(1)/etc/config/mqtt_login
	$(INSTALL_CONF) ./files/mqtt_topics.config $(1)/etc/config/mqtt_topics
endef

$(eval $(call BuildPackage,MQTTSubscriber))