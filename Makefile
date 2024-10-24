include $(TOPDIR)/rules.mk

PKG_NAME:=MQTTBroker
PKG_RELEASE:=1
PKG_VERSION:=1.0.0
PKG_BUILD_DEPENDS:=!USE_GLIBC:argp-standalone

include $(INCLUDE_DIR)/package.mk

define Package/MQTTBroker
	CATEGORY:=Base system
	TITLE:=MQTTBroker
	DEPENDS:=+libmosquitto +libsqlite3 +libuci
endef

ifndef CONFIG_USE_GLIBC
	CONFIGURE_VARS += LIBS="-largp"
endif

define Package/MQTTBroker/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/MQTTBroker $(1)/usr/bin
	$(INSTALL_CONF) ./files/mqtt_login.config $(1)/etc/config/mqtt_login
	$(INSTALL_CONF) ./files/mqtt_topics.config $(1)/etc/config/mqtt_topics
endef

$(eval $(call BuildPackage,MQTTBroker))