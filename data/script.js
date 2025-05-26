async function fetchSensorDataLoop() {
  try {
    const response = await fetch('/sensor');
    if (!response.ok) throw new Error('Netzwerkfehler');

    const data = await response.json();

    document.getElementById('sat').textContent = data.SAT_COUNT;
    document.getElementById('lat').textContent = data.LAT.toFixed(3);
    document.getElementById('lon').textContent = data.LON.toFixed(3);
    document.getElementById('alt').textContent = data.ALT.toFixed(3);
    document.getElementById('temp').textContent = data.TEMP.toFixed(3);
    document.getElementById('magx').textContent = data.MAGX.toFixed(3);
    document.getElementById('magy').textContent = data.MAGY.toFixed(3);
    document.getElementById('magz').textContent = data.MAGZ.toFixed(3);
    document.getElementById('accx').textContent = data.ACCX.toFixed(3);
    document.getElementById('accy').textContent = data.ACCY.toFixed(3);
    document.getElementById('accz').textContent = data.ACCZ.toFixed(3);
    document.getElementById('gyrox').textContent = data.GYROX.toFixed(3);
    document.getElementById('gyroy').textContent = data.GYROY.toFixed(3);
    document.getElementById('gyroz').textContent = data.GYROZ.toFixed(3);
    document.getElementById('tick').textContent = data.TICK;
  } catch (error) {
    console.error('Fehler beim Laden der Sensor-Daten:', error);
  } finally {
    // Wiederhole, aber erst nach vollständigem Abschluss
    setTimeout(fetchSensorDataLoop, 200);
  }
}

fetchSensorDataLoop(); // Start
