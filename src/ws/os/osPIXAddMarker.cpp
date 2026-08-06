// osPIXAddMarker @0x825E5690 — tag the current point in the PIX capture timeline with a named
// marker. An empty name is replaced with the placeholder "No name". Color argument is 0.
extern "C" void PIXSetMarker(unsigned int color, const char *name); // boundary — PIX

void osPIXAddMarker(const char *name)
{
    const char *marker = name;
    if (name[0] == '\0')
        marker = "No name";
    PIXSetMarker(0, marker);
}
