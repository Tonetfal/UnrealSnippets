# Unreal Snippets

This is a collection of snippets for Unreal Engine which are meant to show certain modifications of mine. The snippets
are not going to work out of the box, as it may contain some of the code that is related to my project, which I'm 
leaving as is as it adds some clarity (such as logging), but it shouldn't be a problem to integrate it in your 
project, if you're familiar enough with Unreal Engine and C++.

# Ability System

Ability system snippet present a way of creating generic actions using the Enhanced Input System. The actions are 
meant to be project specific, however there usually are some common generic actions, such as `Confirm` and `Cancel`, 
which are illustrated in the example, and integrated in the `Gameplay Ability` as default events which can be easily 
implemented in either C++ or Blueprints.

## Setup

1) Create an `Input Mapping Context` (IMC) containing all the `Input Actions` (IA) and bound to the actual inputs.
2) Open `Project Settings` -> `MTD` -> `Ability System Settings`.
3) In the settings you have to assign the previously created `IMC`, give it a priority, and associate every `IA` with
   a `Gameplay tag` that will be used to identify that specific ability system generic action.
4) Implement `UMTD_GameplayAbility::OnGenericActionTriggered(FGameplayTag GenericActionTag)` in your custom ability 
   to handle the generic action.
5) (Optional) Create your own `UMTD_GameplayAbility::OnYOURACTIONActionTriggered()` for actions specific to your own 
   game, the same way I did with `UMTD_GameplayAbility::OnConfirmActionTriggered()` and 
   `UMTD_GameplayAbility::OnCancelActionTriggered()`, as they're going to be used a lot.

## Functionality

The generic actions are meant to be triggered (and be trigger-able) only when an ability that wants that to happen 
will be executing.

The `MTD_GameplayAbility` has a simple boolean called `bActivateGenericInputActions` that makes the generic actions 
`IMC` to be bound to the ability avatar's `UEnhancedInputComponent` while the ability is executing. It means that the 
`IMC` is unbound, hence the generic actions are non-trigger-able, after the ability ends.

While the ability that wants to use generic actions is executing all the other `IA`s bound to the very same keys are 
meant to not trigger, as the generic actions `IA`s are meant to consume the input as soon as possible, leaving others
unaware about it.

One of the generic actions the snippet shows is `Confirm`. In my game that action is bound to `Left Mouse Button` and 
`Space Bar`, however these inputs are also associated with other `IA`s, such as `Jump` and `Fire`. However, as 
aforementioned, when the ability is executing other `IA`s are not going to receive any input, so in my case pressing 
`Space Bar` doesn't make the character jump, and clicking `Left Mouse Button` doesn't make the gun to fire, but 
instead it triggers `UMTD_AbilitySystemComponent::OnGenericActionInput(FGameplayTag GenericActionTag)`, which in turn is 
listened by `UMTD_GameplayAbility::OnGenericActionTriggered`, notifying the ability about the `Confirm` generic action. 
Upon finishing the ability we unbind these delegates, making the `IMC` be unbound as well, so that the player can jump 
and fire once again.

# Sticky focus

When working with UI, it's extremely annoying to lose focus because the user clicks on something they are not 
intended to click on. For instance, it might be the background of some menu screen. Usually they can get the focus 
back by pressing on a focusable widget as they usually would, but it might ruin your animations making it feel like 
the user broke something internally. There's a quick workaround that I personally find useful, but it might have some 
flaws I'm not aware of.

* This snippet assumes you to use common UI, but you can get it working with regular UMG/Slate. The only thing that 
  has to be modified is how you define that you want your sticky focus to happen. Look at 
  `AMTD_PlayerController::OnFocusChanged()`.

## Setup

1) Integrate what you can find in `MTD_PlayerController`.
2) There is no step 2.

## Functionality

When the active common UI config is menu and the widget that you focused on is viewport, it saves the last widget 
that was unfocused, and focuses on it the next frame. It's pretty much automatic, so you don't have to add or edit 
anything in your widgets. All you have to do is make sure that you handle your focus properly.