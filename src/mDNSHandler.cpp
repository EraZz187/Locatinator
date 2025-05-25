#include <ESPmDNS.h>
#include "mDNSHandler.h"

void setupMDNS(const char* hostname) {
  if (!MDNS.begin(hostname)) {
    Serial.println("❌ mDNS konnte nicht gestartet werden.");
    return;
  }

  MDNS.addService("http", "tcp", 80);
  Serial.println("✅ mDNS gestartet " + MDNS.hostname(0));
}