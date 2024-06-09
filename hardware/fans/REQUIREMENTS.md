# Crucial fan requirements

The ICX-FAN10-MOD is a specialized design made to work hand-in-hand with the 1U layout to ensure adequate airflow over all components. When changing that, there are a number of absolutely crucial requirements that must be met to ensure components receive adequate cooling. Especially as many higher end models additionally include air guides which are not present in the ICX-FAN10-MOD design.

## Stock Airflow Requirements
In stock configuration, the ICX-FAN10-MOD is much, much more extreme than you realize. 

[This is the newer model of the fan used in the ICX, with a standard pinout.](https://www.delta-fan.com/products/FFB0412UHN-SP4Z.html) And yes, they really are rated to last 382,000 hours at 70C.

More importantly, the fans in the FAN10-MOD are not mounted in a back-to-back or contra-rotating configuration; they are mounted in a *wind tunnel* configuration with wide separation. This means they move significantly more air than the stated specification sheets. Each individual fan module moves approximately **75CFM** for a total of **150CFM**.

To put this into better perspective; the ICX7450-48P is **just 0.275ft^3** if you took out all the innards. This means the fans are moving the entire volume of the chassis **more than 500 times per minute at full speed!** These switches require a tremendous amount of airflow when significantly loaded!

## Addressing this with modifications
Because we are removing critical air guides in some models, we must move EVEN MORE air to ensure things operate correctly. For chassis with a single fan module, this means approximately 80-90CFM. For a two module configuration, this increases to a general recommendation of at least 180-200CFM with a preference toward higher static pressure.

Which isn't as bad as you think. For example, one of the go-to fans is the Arctic Bionix P140. Each of these moves 104CFM. Because we are in an 'open' configuration with no restriction or guides, we can simply use both the CFM number and static pressure graphs as is. So two of them can handle a maximum configuration 7450 with no issues at all!

It is also important to note that the stock fans are relatively high current, to the point where the design incorporates load resistors to prevent the switch from faulting on current sense. Each FAN10-MOD slot expects two fans at 0.92A each, for a total of 1.84A! Most of the fans recommended are less than 0.3A each. 

## "I want to use (3, 4, more) fans!"
You can daisy chain with things like Arctic's iPST, or splitters, but only one tach and only one PWM signal will be carried from a single fan. For this reason, **do not ever mix and match fans**. Load must also ALWAYS be equally split between the FAN1 and FAN2 headers. If using an odd number of fans, the higher load should be placed on FAN2. 

IMPORTANT: FAN1 and FAN2 are NOT shared load; they correspond directly to the slots! EACH fan connector is directly connected to each module's 12V rail. The switch DOES incorporate a current sense circuit in addition to the presence signal. Only the controller load is shared.

Note that if you intend to use more than 2 fans, **PCB modifications may be required!** Carefully calculate the peak load of all fans, then add 0.7A for the ICX-FAN10-CTL itself. If this exceeds 1.6A, you must *REMOVE* the startup load resistors (R1 and R2.)

Example Calculation:
2 x Arctic Bionix P140 = 0.4A, must be second in chain
2 x Arctic S4028-6K = 0.2A, must be first in chain
Total = 0.6A, no modifications required!

# YOU CAN'T JUST USE ANY FANS!

This is going to be repeated ad infinitum, because it really is that important. You cannot just use any fans to do this mod. Sure, your temperatures might look okay. But the chassis actually has a very bad sensor setup; it can only monitor the Tomahawk core and a single NTC thermistor. (The PSUs provide data from internal thermistors, but the software doesn't show it.)

Everybody's favorite brown fans? **Mostly cannot be used.** Fans are the art of compromise; reducing noise means sacrificing air flow or static pressure. Noctua chose both. **DO NOT USE A12's. EVER.**

## Recommended Fans
### High Performance, Low Noise
* Arctic Bionix P140 (PWM, iPST)
* Arctic P14 PWM PST (PWM, iPST)
* Noctua NF-A14 industrialPPC-2000 PWM (PWM)
* Fractal Design Silent R3 140mm (PWM)

## Absolutely Unacceptable Fans
* Noctua NF-A12 series (all models)
* Noctua NF-P12 series (all models)
* Fractal Design Silent R3 120mm (PWM) - actual CFM is below 45!