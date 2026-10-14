
import sys
import json
import base64
from io import BytesIO

import numpy as np
import pydicom
from pydicom.pixels import apply_modality_lut, apply_voi_lut
from PIL import Image


def png_base64(image):
    buffer = BytesIO()
    image.save(buffer, format="PNG")
    return base64.b64encode(buffer.getvalue()).decode("ascii")


def render_dicom(ds):
    """Create a display image from a conventional grayscale DICOM."""
    pixels = ds.pixel_array
    if pixels.ndim != 2:
        raise ValueError(
            "This prototype supports one 2D grayscale image only."
        )

    pixels = apply_modality_lut(pixels, ds)

    try:
        pixels = apply_voi_lut(pixels, ds)
    except (ValueError, TypeError):
        pass

    pixels = np.asarray(pixels, dtype=np.float32)
    valid = np.isfinite(pixels)

    if not valid.any():
        raise ValueError("DICOM image contains no valid pixels.")

    low, high = np.percentile(pixels[valid], [1, 99])
    if high <= low:
        high = low + 1

    display = np.clip((pixels - low) / (high - low), 0, 1)
    display = (display * 255).astype(np.uint8)

    if ds.get("PhotometricInterpretation", "") == "MONOCHROME1":
        display = 255 - display

    return pixels, display


def run_segmentation(pixels):
    """
    DEMO ONLY: segment high-intensity pixels.

    Replace this function with your actual segmentation model.
    `pixels` contains the modality-transformed image values.
    Return a boolean or 0/1 mask with the same H x W shape.
    """
    threshold = np.percentile(pixels, 90)
    return pixels > threshold


def main():
    if len(sys.argv) != 2:
        raise ValueError("Usage: segment_dicom.py <dicom-file>")

    ds = pydicom.dcmread(sys.argv[1])
    print(
    f"DICOM dimensions: {ds.Rows} rows x {ds.Columns} columns",
    file=sys.stderr
    )

    pixels, display = render_dicom(ds)

    print(
    f"Output dimensions: {display.shape[0]} rows x {display.shape[1]} columns",
    file=sys.stderr
    )

    # Run the segmentation algorithm.
    mask = np.asarray(run_segmentation(pixels), dtype=bool)

    if mask.shape != display.shape:
        raise ValueError("Segmentation mask dimensions do not match.")

    original = Image.fromarray(display, mode="L")

    # Transparent red overlay; only segmented pixels are visible.
    rgba = np.zeros((*mask.shape, 4), dtype=np.uint8)
    rgba[mask] = [255, 40, 40, 150]
    overlay = Image.fromarray(rgba, mode="RGBA")

    print(json.dumps({
        "original": png_base64(original),
        "overlay": png_base64(overlay),
        "width": int(display.shape[1]),
        "height": int(display.shape[0])
    }))


if __name__ == "__main__":
    try:
        main()
    except Exception as exc:
        print(json.dumps({"error": str(exc)}), file=sys.stderr)
        sys.exit(1)