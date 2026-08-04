**Halo: Combat Evolved is owned by Microsoft**, (Xbox Game Studios).

**This project is not affiliated with Microsoft.** Microsoft holds all legal rights to Halo: CE, including the material this project is based on.

This is released in good faith, the source material has been public for many years now (hosted on the hidden palace) and doesn't appear to have been contested by Microsoft.

The intended users/researches of this project must have legally purchased access to the game.

You cannot play Halo with this material.

---

Halo 1/Combat Evolved (CE) source code.

This is a corpus to help in-progress Halo: Combat Evolved decompilation efforts, particularly projects using LLMs, to fast track their progress with another point of reference.

It is a point of reference for earlier Halo CE (Halo Xbox NTCS/PAL/PC Custom Edition) and more modern Halo CE (Halo MCC variants) decomplication efforts.

Context:

[Halo: Combat Evolved Anniversary (Jun 24, 2011 prototype) - Hidden Palace](https://hiddenpalace.org/Halo:_Combat_Evolved_Anniversary_\(Jun_24,_2011_prototype\))
This prototype of Halo CE Anniversary has _very_ verbose symbols with minimal compile time optimization.
All dependancy libraries were statically linked.

`HCEX_Release.xex` is the targeted binary with associated symbol data.

HCEA (Saber Engine) Version is `0.0.83147`
HCE (Blam) Version is `01.00.01.0563`

reclaimers.net has flow chart of Blame engine variants over releases:
[Halo 1 - c20](https://c20.reclaimers.net/h1/)

This corpus is the result of pulling out all the static library information, symbols, types, etc, into something useful.

Blam (Halo CE engine) was the target, the entire Blam engine library is actually statically linked in so even functions that the game engine doesn't call during gameplay were compiled in, everything has been re-sourced. There were some toolkit calls and `#ifdefs` that ended up no-opíng a few functions calls however.

Havok and some Xbox360 libraries were only partially re-sourced. I only re-sourced what I needed to complete edges so I could compile TUs. The Havok framework statically linked here is in excess of 12,000 functions.

Saber's HCEA management engine system has been re-sourced.
Saber's entire engine code (Halo CEA doesn't use the full engine) is statically linked in this binary, I pulled a bit more of the AI system out during my tests but it's out of scope for this project. All of the Saber sub systems Halo CEA uses are re-sourced completely.

As of Aug 2026, there are a few in-progress campaigns aiming for byte level accuracy for the original Xbox Halo NTCS and PAL releases of Halo CE:

https://github.com/stianeklund/halo/
https://github.com/bnunu/halo/

Which are forks of:
https://github.com/halo-re/halo (join the Discord of this project, all of the project contributors of various halo decomp projects are in here)

And other efforts:
https://github.com/punpckhdq/halo
https://github.com/Aerocatia/demon

There are several other in-flight Blam engine research projects I'm also aware of but the list would be huge. I hope this project is useful to all of them.

**Once these campaigns are completed, this Github project should be discarded/archived as this was just my own little experiment to see how effective LLMs are at re-sourcing binaries (in this case an Xbox 360, PowerPC bin). Complete binary accuracy wasn't the intent of this corpus, although you'll find its near close to it, the other projects have this in scope as a hard requirement with various proof of work tools.

Other relevant information:

The source is human readable so you're welcome to browse away and learn more about Blam and Halo CE!

Significant effort was put into the symbol recovery, the source should be very verbose with symbols and a minimal amount of magicnumbers.

If you would like some Fable produced documentation on the Blam engine, check out https://surreptitiousresearch.github.io/halocea_docs/.
This might be more accessible for those who don't like looking at code, although I feel the code is very readable, even for those who don't program! I know it's easy to slander the docs as AI Slop, but I must admit, the frontier models are in a league of their own now with no signs of slowing down their intelligence improvements year-on-year.
The AI subsystem deep dive has the most verbose docs as it's the part of Blam I wanted to pick apart the most.

A variety of different anthropic models were used as I was experimenting with their performance with different effort modes as I prosecuted the binary:
Specifically Haiku 4.5, Sonnet 5, Opus 4.8, Opus 5 and Fable 5.
There were some side effects of switching models, no persistant or lingering issues as I've now completed several iterations to get a reasonable amount of conformity in the project output. I have 100s of lessons learned so I consider this project a complete success for what I wanted out of it.

This project took me about 5 weeks on and off.
Getting to %80 of completion was about 2 weeks, the other 3 weeks spent were grooming the symbols, magic numbers, types, etc. 

Final state: fullcompile C side 8,937 TUs / C++ side 4,638 TUs.

When and where possible, inline functions were unrolled back into function calls, the source is attempting to resemble what Bungie developers were working with. That said, the C MACROs are not what Bungie were using, this is a design choice I made to fast track some token/scripting optimizers. See `bungie_datum_access.c` on how Bungie set up thier C MACROs.

There are no optimization's in the source code, all re-constructions have a concrete reference to the .xex data, your LLM should be able to find its way through the source and correlate the hard evidence quickly.

Wishing everyone the best with their own decompile efforts, I'm quite excited to see the original Halo CE source for Xbox and other Halo games.

You don't need to credit this repo or myself if you use anything here.

Pour a can of your favorite beverage out (mines Redbull at the moment) for idasql (https://github.com/allthingsida/idasql) which did all the hard work.. I'm just a New Zealander banging a hammer around.

[Eric Trautmann: "The “117” reference](https://web.archive.org/web/20201112031126/https://twitter.com/mercuryeric/status/883833638701355008)
John 117 -> Revelation 1:17
"When I saw him, I fell at his feet as though dead. Then he placed his right hand on me and said: “Do not be afraid. I am the First and the Last."

Jesus Christ is the Lord, God Bless to whomever reads this.