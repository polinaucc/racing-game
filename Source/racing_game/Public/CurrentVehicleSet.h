#pragma once
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Templates/Tuple.h"
#include "CurrentVehicleSet.generated.h"

UCLASS()
class RACING_GAME_API UCurrentVehicleSet : public UUserWidget {
	GENERATED_BODY()

	using ButtonHandle = TTuple<class UButton*, class UHorizontalBoxSlot*>;
protected:
	TArray<TArray<ButtonHandle>> buttons;
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> colors;
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> ammo;
public:
	//UCurrentVehicleSet(FObjectInitializer const& oi);
	virtual TSharedRef<SWidget> RebuildWidget() override;
	class UCanvasPanel* update_root();
	void update_widgets();
	void update(bool full = false);

	enum class button_type {
		active, inactive, unavailable
	};
	TArray<ButtonHandle> item(UPanelWidget *panel, int index, FString name, FString title, TArray<bool> availability, int current);
	ButtonHandle button(UPanelWidget *panel, button_type type, FString name, int number, int row);
	void update_button(ButtonHandle b, button_type new_type);

	ButtonHandle color(UPanelWidget *panel, FString name, int number);
	ButtonHandle color_button(UPanelWidget *panel, button_type type, FString name, int number);
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> color_item(UPanelWidget *panel, TArray<bool> availability, int current, FString name);
	void update_color_button(ButtonHandle b, button_type new_type);

	ButtonHandle ammo_icon(UPanelWidget *panel, FString name, int number);
	ButtonHandle ammo_button(UPanelWidget *panel, button_type type, FString name, int number);
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> ammo_item(UPanelWidget *panel, TArray<bool> availability, int current, FString name);

	void button_event(int row, int item);
	void color_event(int item);
	void ammo_event(int item);
	void enable_events(UButton *b, int row, int item);
	void enable_color_events(UButton *b, int item);
	void enable_ammo_events(UButton *b, int item);

	UFUNCTION(BlueprintImplementableEvent)
	void PurchaseEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void NotEnoughMoneyEvent();
	UFUNCTION(BlueprintImplementableEvent)
	void ColorChangeEvent();

	UFUNCTION() void button_0_0();
	UFUNCTION() void button_0_1();
	UFUNCTION() void button_0_2();
	UFUNCTION() void button_0_3();
	UFUNCTION() void button_0_4();

	UFUNCTION() void button_1_0();
	UFUNCTION() void button_1_1();
	UFUNCTION() void button_1_2();
	UFUNCTION() void button_1_3();
	UFUNCTION() void button_1_4();

	UFUNCTION() void button_2_0();
	UFUNCTION() void button_2_1();
	UFUNCTION() void button_2_2();
	UFUNCTION() void button_2_3();
	UFUNCTION() void button_2_4();

	UFUNCTION() void button_3_0();
	UFUNCTION() void button_3_1();
	UFUNCTION() void button_3_2();
	UFUNCTION() void button_3_3();
	UFUNCTION() void button_3_4();

	UFUNCTION() void button_4_0();
	UFUNCTION() void button_4_1();
	UFUNCTION() void button_4_2();
	UFUNCTION() void button_4_3();
	UFUNCTION() void button_4_4();

	UFUNCTION() void button_5_0();
	UFUNCTION() void button_5_1();
	UFUNCTION() void button_5_2();
	UFUNCTION() void button_5_3();
	UFUNCTION() void button_5_4();

	UFUNCTION() void button_6_0();
	UFUNCTION() void button_6_1();
	UFUNCTION() void button_6_2();
	UFUNCTION() void button_6_3();
	UFUNCTION() void button_6_4();

	UFUNCTION() void button_7_0();
	UFUNCTION() void button_7_1();
	UFUNCTION() void button_7_2();
	UFUNCTION() void button_7_3();
	UFUNCTION() void button_7_4();

	UFUNCTION() void button_8_0();
	UFUNCTION() void button_8_1();
	UFUNCTION() void button_8_2();
	UFUNCTION() void button_8_3();
	UFUNCTION() void button_8_4();

	UFUNCTION() void button_9_0();
	UFUNCTION() void button_9_1();
	UFUNCTION() void button_9_2();
	UFUNCTION() void button_9_3();
	UFUNCTION() void button_9_4();

	UFUNCTION() void button_c_0();
	UFUNCTION() void button_c_1();
	UFUNCTION() void button_c_2();
	UFUNCTION() void button_c_3();
	UFUNCTION() void button_c_4();
	UFUNCTION() void button_c_5();
	UFUNCTION() void button_c_6();
	UFUNCTION() void button_c_7();
	UFUNCTION() void button_c_8();
	UFUNCTION() void button_c_9();

	UFUNCTION() void button_a_0();
	UFUNCTION() void button_a_1();
	UFUNCTION() void button_a_2();
};
