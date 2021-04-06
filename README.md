# smartTBW
Small utility like smartmontools but just for NVMe SSDs on macOS

No command line parameters yet. I will probably add a JSON output sometime soon. Only works on an NVMe SSD equiped Mac that is supported by Apple's NVMe SMART library. This is intentionally made a simple as possible to make it easier to verify that the values being returned are valid. There has been suspicion that the results from smartmontools and Drive DX aren't returning accurate data. This should be enough for any skeptics to see that the Apple NVMe SMART library does indeed return valid results.

I welcome any code reviews to verify the code.
