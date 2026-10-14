
const button = document.getElementById('segmentButton');
const status = document.getElementById('status');
const original = document.getElementById('originalImage');
const overlay = document.getElementById('segmentationOverlay');

new QWebChannel(qt.webChannelTransport, (channel) => {
  const bridge = channel.objects.dicomBridge;
  bridge.segmentationReady.connect((originalUrl, overlayUrl) => {
    original.onload = () => {
      status.textContent = 'Segmentation complete';
      button.disabled = false;
    };

    original.onerror = () => {
      status.textContent = 'Unable to display image';
      button.disabled = false;
    };

    original.src = originalUrl;
    overlay.src = overlayUrl;
  });

  bridge.errorOccurred.connect((message) => {
    status.textContent = `Error: ${message}`;
    button.disabled = false;
  });

  button.addEventListener('click', () => {
    button.disabled = true;
    status.textContent = 'Running segmentation...';
    bridge.selectAndSegment();
  });
});