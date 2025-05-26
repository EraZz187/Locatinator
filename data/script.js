async function fetchSensorData() {
  try {
    const response = await fetch('/sensor');
    if (!response.ok) throw new Error('Netzwerkfehler');

    const data = await response.json();

    document.getElementById('sat').textContent = data.SAT_COUNT;
    document.getElementById('lat').textContent = data.LAT;
    document.getElementById('lon').textContent = data.LON;
    document.getElementById('alt').textContent = data.ALT;

    document.getElementById('temp').textContent = data.TEMP;

    document.getElementById('magx').textContent = data.MAGX;
    document.getElementById('magy').textContent = data.MAGY;
    document.getElementById('magz').textContent = data.MAGZ;

    document.getElementById('accx').textContent = data.ACCX;
    document.getElementById('accy').textContent = data.ACCY;
    document.getElementById('accz').textContent = data.ACCZ;

    document.getElementById('gyrox').textContent = data.GYROX;
    document.getElementById('gyroy').textContent = data.GYROY;
    document.getElementById('gyroz').textContent = data.GYROZ;
    
    document.getElementById('tick').textContent = data.TICK;

  } catch (error) {
    console.error('Fehler beim Laden der Sensor-Daten:', error);
  }
}


setInterval(fetchSensorData, 5000); // Daten alle 5 Sekunden aktualisieren
fetchSensorData(); // Direkt nach Laden der Seite initial laden