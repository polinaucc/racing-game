#include "CurrentVehicleSet.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Brushes/SlateImageBrush.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Misc/Paths.h"
#include "Save.h"
#include <stdexcept>

TSharedRef<SWidget> UCurrentVehicleSet::RebuildWidget() {
	UPanelWidget* root = Cast<UPanelWidget>(GetRootWidget());
	if (!root)
		WidgetTree->RootWidget = update_root();

	auto ret = Super::RebuildWidget();
	update_widgets();
	return ret;
}
UCanvasPanel* UCurrentVehicleSet::update_root() {
	auto root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("RootCanvas"));
	auto slot = Cast<UCanvasPanelSlot>(root->Slot);
	if (slot)
		slot->SetOffsets(FMargin(100, 100));
	return root;
}
void UCurrentVehicleSet::update_widgets() {
	UPanelWidget* root = Cast<UPanelWidget>(GetRootWidget());
	if (root && WidgetTree) {
		USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
		Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
		if (!Savefile)
			Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

		int i = 0;
		buttons.Empty();
		buttons.Add(item(root, i++, "rpm", "Engine RPM", Savefile->EngineRPM, switch_EEquipementLevel(Savefile->CurrentEngineRPM)));
		buttons.Add(item(root, i++, "gear", "Gear Switch Time", Savefile->GearSwitchTime, switch_EEquipementLevel(Savefile->CurrentGearSwitchTime)));
		buttons.Add(item(root, i++, "mass", "Mass", Savefile->Mass, switch_EEquipementLevel(Savefile->CurrentMass)));
		buttons.Add(item(root, i++, "drag", "Drag Coefficient", Savefile->DragCoefficient, switch_EEquipementLevel(Savefile->CurrentDragCoefficient)));
		buttons.Add(item(root, i++, "steer", "Maximum Steering Angle", Savefile->MaximumSteerAngle, switch_EEquipementLevel(Savefile->CurrentMaximumSteerAngle)));
		buttons.Add(item(root, i++, "tire", "Tire Friction", Savefile->FrictionScale, switch_EEquipementLevel(Savefile->CurrentFrictionScale)));
		buttons.Add(item(root, i++, "stiff", "Lateral Stiffness", Savefile->LatStiffness, switch_EEquipementLevel(Savefile->CurrentLatStiffness)));
		buttons.Add(item(root, i++, "arm", "Armor", Savefile->Armor, switch_EEquipementLevel(Savefile->CurrentArmor)));
		buttons.Add(item(root, i++, "dam", "Damage", Savefile->Damage, switch_EEquipementLevel(Savefile->CurrentDamage)));
		buttons.Add(item(root, i++, "hp", "Maximum Health", Savefile->MaxHealth, switch_EEquipementLevel(Savefile->CurrentMaxHealth)));

		colors.Get<0>().Empty();
		colors.Get<1>().Empty();
		colors = color_item(root, Savefile->Paint, Savefile->CurrentPaint, "color");

		ammo.Get<0>().Empty();
		ammo.Get<1>().Empty();
		ammo = ammo_item(root, Savefile->Ammo, Savefile->CurrentAmmo, "ammo");
	}
}
void UCurrentVehicleSet::update(bool full) {
	UPanelWidget* root = Cast<UPanelWidget>(GetRootWidget());
	if (full || !root)
		WidgetTree->RootWidget = update_root();
	update_widgets();
}

UCurrentVehicleSet::ButtonHandle UCurrentVehicleSet::button(UPanelWidget *panel, button_type type, FString name, int number, int row) {
	name.AppendInt(number);
	switch (type) {
		case button_type::active:
			name.AppendChar('a');
			break;
		case button_type::inactive:
			name.AppendChar('n');
			break;
		case button_type::unavailable:
			name.AppendChar('d');
			break;
	}
	auto ret = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*name));
	auto slot = Cast<UHorizontalBoxSlot>(panel->AddChild(ret));
	slot->SetSize(ESlateSizeRule::Fill);
	slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	slot->SetPadding(FMargin(0));

	auto ret_tuple = MakeTuple(ret, slot);
	update_button(ret_tuple, type);
	enable_events(ret, row, number);
	return ret_tuple;
}
void UCurrentVehicleSet::update_button(ButtonHandle b, button_type new_type) {
	switch (new_type) {
		case button_type::active:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/p_empty.p_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/p_empty.p_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/p_empty.p_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
		case button_type::inactive:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/n_empty.n_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/h_empty.h_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/p_empty.p_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
		case button_type::unavailable:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/n_disabled.n_disabled'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/h_disabled.h_disabled'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/p_empty.p_empty'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
	}
}
TArray<UCurrentVehicleSet::ButtonHandle> UCurrentVehicleSet::item(UPanelWidget *panel, int index, FString name, FString title, TArray<bool> availability, int current) {
	auto box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*name));
	auto box_slot = Cast<UCanvasPanelSlot>(panel->AddChild(box));
	box_slot->SetSize(FVector2D(375, 75));
	box_slot->SetPosition(FVector2D(1920 - 100 - 325, index * 80 + 150));

	TArray<ButtonHandle> ret;
	if (current == 0)
		ret.Add(button(box, button_type::active, name + TEXT("_"), 0, index));
	else
		ret.Add(button(box, button_type::inactive, name + TEXT("_"), 0, index));

	for (int i = 1; i < availability.Num() + 1; i++)
		if (current == i)
			ret.Add(button(box, button_type::active, name + TEXT("_"), i, index));
		else if (availability[i - 1])
			ret.Add(button(box, button_type::inactive, name + TEXT("_"), i, index));
		else
			ret.Add(button(box, button_type::unavailable, name + TEXT("_"), i, index));

	auto text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName(*(name + TEXT("_text"))));
	auto text_slot = Cast<UCanvasPanelSlot>(panel->AddChild(text));
	text_slot->SetSize(FVector2D(400 - 15, 75));
	text_slot->SetPosition(FVector2D(1920 - 100 - 325 - 400, index * 80 + 150 + 25));

	text->SetText(FText::FromString(title + TEXT(": ")));
	text->SetJustification(ETextJustify::Right);

	return ret;
}

FString generate(FString number) {
	return TEXT("Texture2D'/Game/Menu/Images/Colors/c") + number + TEXT(".c") + number + TEXT("'");
}
UCurrentVehicleSet::ButtonHandle UCurrentVehicleSet::color(UPanelWidget *panel, FString name, int number) {
	name.AppendInt(number);
	name.AppendChar('c');

	auto ret = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*name));
	auto slot = Cast<UHorizontalBoxSlot>(panel->AddChild(ret));
	slot->SetSize(ESlateSizeRule::Fill);
	slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	slot->SetPadding(FMargin(0));

	auto num = FString::FromInt(number);
	FSlateImageBrush t(Cast<UObject>(LoadObject<UTexture2D>(NULL, *generate(num), NULL, LOAD_None, NULL)), FVector2D(75, 75));
	ret->WidgetStyle.SetNormal(t);
	ret->WidgetStyle.SetHovered(t);
	ret->WidgetStyle.SetPressed(t);

	return MakeTuple(ret, slot);
}
UCurrentVehicleSet::ButtonHandle UCurrentVehicleSet::color_button(UPanelWidget *panel, button_type type, FString name, int number) {
	name.AppendInt(number);
	switch (type) {
		case button_type::active:
			name.AppendChar('a');
			break;
		case button_type::inactive:
			name.AppendChar('n');
			break;
		case button_type::unavailable:
			name.AppendChar('d');
			break;
	}
	auto ret = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*name));
	auto slot = Cast<UHorizontalBoxSlot>(panel->AddChild(ret));
	slot->SetSize(ESlateSizeRule::Fill);
	slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	slot->SetPadding(FMargin(0));

	auto ret_tuple = MakeTuple(ret, slot);
	update_color_button(ret_tuple, type);
	enable_color_events(ret, number);
	return ret_tuple;
}
void UCurrentVehicleSet::update_color_button(ButtonHandle b, button_type new_type) {
	switch (new_type) {
		case button_type::active:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/an.an'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/ah.ah'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/ap.ap'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
		case button_type::inactive:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/nn.nn'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/nh.nh'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/np.np'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
		case button_type::unavailable:
		{
			FSlateImageBrush n(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/dn.dn'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush h(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/dh.dh'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			FSlateImageBrush p(Cast<UObject>(LoadObject<UTexture2D>(NULL, TEXT("Texture2D'/Game/Menu/Images/Colors/dp.dp'"), NULL, LOAD_None, NULL)), FVector2D(75, 75));
			b.Get<0>()->WidgetStyle.SetNormal(n);
			b.Get<0>()->WidgetStyle.SetHovered(h);
			b.Get<0>()->WidgetStyle.SetPressed(p);
			break;
		}
	}
}
TTuple<TArray<UCurrentVehicleSet::ButtonHandle>, TArray<UCurrentVehicleSet::ButtonHandle>> UCurrentVehicleSet::color_item(UPanelWidget *panel, TArray<bool> availability, int current, FString name) {
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> ret;

	auto color_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*name));
	auto color_box_slot = Cast<UCanvasPanelSlot>(panel->AddChild(color_box));
	color_box_slot->SetSize(FVector2D((availability.Num() + 1) * 75, 75));
	color_box_slot->SetPosition(FVector2D(585, 1));

	for (int i = 0; i < availability.Num() + 1; i++)
		ret.Get<1>().Add(color(color_box, name + "_c_", i));

	auto button_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*name));
	auto button_box_slot = Cast<UCanvasPanelSlot>(panel->AddChild(button_box));
	button_box_slot->SetSize(FVector2D((availability.Num() + 1) * 75, 75));
	button_box_slot->SetPosition(FVector2D(585, 0));

	if (current == 0)
		ret.Get<0>().Add(color_button(button_box, button_type::active, name + "_", 0));
	else
		ret.Get<0>().Add(color_button(button_box, button_type::inactive, name + "_", 0));

	for (int i = 1; i < availability.Num() + 1; i++)
		if (current == i)
			ret.Get<0>().Add(color_button(button_box, button_type::active, name + "_", i));
		else if (availability[i - 1])
			ret.Get<0>().Add(color_button(button_box, button_type::inactive, name + "_", i));
		else
			ret.Get<0>().Add(color_button(button_box, button_type::unavailable, name + "_", i));

	return ret;
}

FString icon_name(int number) {
	switch (number) {
		case 0: return TEXT("Texture2D'/Game/Menu/Images/Weapons/machine_gun.machine_gun'");
		case 1: return TEXT("Texture2D'/Game/Menu/Images/Weapons/multi_gun.multi_gun'");
		case 2: return TEXT("Texture2D'/Game/Menu/Images/Weapons/big_gun.big_gun'");
		default: throw std::runtime_error("Unsupported enum value.");
	}
}
UCurrentVehicleSet::ButtonHandle UCurrentVehicleSet::ammo_icon(UPanelWidget *panel, FString name, int number) {
	name.AppendInt(number);
	name.AppendChar('i');

	auto ret = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*name));
	auto slot = Cast<UHorizontalBoxSlot>(panel->AddChild(ret));
	slot->SetSize(ESlateSizeRule::Fill);
	slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	slot->SetPadding(FMargin(0));

	auto num = FString::FromInt(number);
	FSlateImageBrush t(Cast<UObject>(LoadObject<UTexture2D>(NULL, *icon_name(number), NULL, LOAD_None, NULL)), FVector2D(75, 75));
	ret->WidgetStyle.SetNormal(t);
	ret->WidgetStyle.SetHovered(t);
	ret->WidgetStyle.SetPressed(t);

	return MakeTuple(ret, slot);
}
UCurrentVehicleSet::ButtonHandle UCurrentVehicleSet::ammo_button(UPanelWidget *panel, button_type type, FString name, int number) {
	name.AppendInt(number);
	switch (type) {
		case button_type::active:
			name.AppendChar('a');
			break;
		case button_type::inactive:
			name.AppendChar('n');
			break;
		case button_type::unavailable:
			name.AppendChar('d');
			break;
	}
	auto ret = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), FName(*name));
	auto slot = Cast<UHorizontalBoxSlot>(panel->AddChild(ret));
	slot->SetSize(ESlateSizeRule::Fill);
	slot->SetVerticalAlignment(EVerticalAlignment::VAlign_Fill);
	slot->SetPadding(FMargin(0));

	auto ret_tuple = MakeTuple(ret, slot);
	update_color_button(ret_tuple, type);
	enable_ammo_events(ret, number);
	return ret_tuple;
}
TTuple<TArray<UCurrentVehicleSet::ButtonHandle>, TArray<UCurrentVehicleSet::ButtonHandle>> UCurrentVehicleSet::ammo_item(UPanelWidget *panel, TArray<bool> availability, int current, FString name) {
	TTuple<TArray<ButtonHandle>, TArray<ButtonHandle>> ret;

	auto ammo_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*name));
	auto ammo_box_slot = Cast<UCanvasPanelSlot>(panel->AddChild(ammo_box));
	ammo_box_slot->SetSize(FVector2D((availability.Num() + 1) * 75, 75));
	ammo_box_slot->SetPosition(FVector2D((1920 - (availability.Num() + 1) * 75) / 2, 1080 - 100));

	for (int i = 0; i < availability.Num() + 1; i++)
		ret.Get<1>().Add(ammo_icon(ammo_box, name + "_i_", i));

	auto button_box = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*name));
	auto button_box_slot = Cast<UCanvasPanelSlot>(panel->AddChild(button_box));
	button_box_slot->SetSize(FVector2D((availability.Num() + 1) * 75, 75));
	button_box_slot->SetPosition(FVector2D((1920 - (availability.Num() + 1) * 75) / 2, 1080 - 100));

	if (current == 0)
		ret.Get<0>().Add(ammo_button(button_box, button_type::active, name + "_", 0));
	else
		ret.Get<0>().Add(ammo_button(button_box, button_type::inactive, name + "_", 0));

	for (int i = 1; i < availability.Num() + 1; i++)
		if (current == i)
			ret.Get<0>().Add(ammo_button(button_box, button_type::active, name + "_", i));
		else if (availability[i - 1])
			ret.Get<0>().Add(ammo_button(button_box, button_type::inactive, name + "_", i));
		else
			ret.Get<0>().Add(ammo_button(button_box, button_type::unavailable, name + "_", i));

	return ret;
}

void UCurrentVehicleSet::button_event(int row, int item) {
	USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
	if (!Savefile)
		Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

	if (switch_EEquipementLevel(Savefile->CurrentByID(row)) == item)
		return; //Already selected.

	auto level = make_EEquipementLevel(item);
	auto old = switch_EEquipementLevel(Savefile->CurrentByID(row));
	if (item == 0)
		Savefile->CurrentByID(row) = EEquipementLevel::L0;
	else if (Savefile->AvailabilityByID(row)[item - 1])
		Savefile->CurrentByID(row) = level;
	else
		if (Savefile->Money >= Savefile->Price(level)) {
			Savefile->AvailabilityByID(row)[item - 1] = true;
			Savefile->CurrentByID(row) = level;
			Savefile->Money -= Savefile->Price(level);
		} else {
			NotEnoughMoneyEvent();
			return;
		}
	update_button(buttons[row][old], button_type::inactive);
	update_button(buttons[row][item], button_type::active);
	UGameplayStatics::SaveGameToSlot(Savefile, Savefile->SaveSlotName, Savefile->UserIndex);
	PurchaseEvent();
}
void UCurrentVehicleSet::color_event(int item) {
	USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
	if (!Savefile)
		Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

	if (Savefile->CurrentPaint == item)
		return; //Already selected.

	//auto level = make_EEquipementLevel(item);
	auto old = Savefile->CurrentPaint;
	if (item == 0)
		Savefile->CurrentPaint = 0;
	else if (Savefile->Paint[item - 1])
		Savefile->CurrentPaint = item;
	else
		if (Savefile->Money >= Savefile->PaintPrice) {
			Savefile->Paint[item - 1] = true;
			Savefile->CurrentPaint = item;
			Savefile->Money -= Savefile->PaintPrice;
		} else {
			NotEnoughMoneyEvent();
			return;
		}
	update_color_button(colors.Get<0>()[old], button_type::inactive);
	update_color_button(colors.Get<0>()[item], button_type::active);
	UGameplayStatics::SaveGameToSlot(Savefile, Savefile->SaveSlotName, Savefile->UserIndex);
	PurchaseEvent();
	ColorChangeEvent();
}
void UCurrentVehicleSet::ammo_event(int item) {
	USave* Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));
	Savefile = Cast<USave>(UGameplayStatics::LoadGameFromSlot(Savefile->SaveSlotName, Savefile->UserIndex));
	if (!Savefile)
		Savefile = Cast<USave>(UGameplayStatics::CreateSaveGameObject(USave::StaticClass()));

	if (Savefile->CurrentAmmo == item)
		return; //Already selected.

	//auto level = make_EEquipementLevel(item);
	auto old = Savefile->CurrentAmmo;
	if (item == 0)
		Savefile->CurrentAmmo = 0;
	else if (Savefile->Ammo[item - 1])
		Savefile->CurrentAmmo = item;
	else
		if (Savefile->Money >= Savefile->AmmoPrice) {
			Savefile->Ammo[item - 1] = true;
			Savefile->CurrentAmmo = item;
			Savefile->Money -= Savefile->AmmoPrice;
		} else {
			NotEnoughMoneyEvent();
			return;
		}
	update_color_button(ammo.Get<0>()[old], button_type::inactive);
	update_color_button(ammo.Get<0>()[item], button_type::active);
	UGameplayStatics::SaveGameToSlot(Savefile, Savefile->SaveSlotName, Savefile->UserIndex);
	PurchaseEvent();
}

void UCurrentVehicleSet::enable_events(UButton *b, int row, int item) {
	switch (row) {
		case 0:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_0_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_0_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_0_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_0_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_0_4);
			}
		case 1:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_1_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_1_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_1_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_1_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_1_4);
			}
		case 2:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_2_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_2_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_2_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_2_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_2_4);
			}
		case 3:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_3_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_3_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_3_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_3_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_3_4);
			}
		case 4:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_4_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_4_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_4_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_4_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_4_4);
			}
		case 5:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_5_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_5_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_5_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_5_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_5_4);
			}
		case 6:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_6_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_6_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_6_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_6_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_6_4);
			}
		case 7:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_7_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_7_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_7_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_7_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_7_4);
			}
		case 8:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_8_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_8_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_8_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_8_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_8_4);
			}
		case 9:
			switch (item) {
				case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_9_0);
				case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_9_1);
				case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_9_2);
				case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_9_3);
				case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_9_4);
			}
	}
	throw std::runtime_error("Unknown button event");
}
void UCurrentVehicleSet::enable_color_events(UButton *b, int item) {
	switch (item) {
		case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_0);
		case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_1);
		case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_2);
		case 3:	return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_3);
		case 4: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_4);
		case 5: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_5);
		case 6: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_6);
		case 7: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_7);
		case 8: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_8);
		case 9: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_c_9);
	}
	throw std::runtime_error("Unknown button event");
}
void UCurrentVehicleSet::enable_ammo_events(UButton *b, int item) {
	switch (item) {
		case 0: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_a_0);
		case 1: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_a_1);
		case 2: return b->OnClicked.AddDynamic(this, &UCurrentVehicleSet::button_a_2);
	}
	throw std::runtime_error("Unknown button event");
}

void UCurrentVehicleSet::button_0_0() { button_event(0, 0); }
void UCurrentVehicleSet::button_0_1() { button_event(0, 1); }
void UCurrentVehicleSet::button_0_2() { button_event(0, 2); }
void UCurrentVehicleSet::button_0_3() { button_event(0, 3); }
void UCurrentVehicleSet::button_0_4() { button_event(0, 4); }

void UCurrentVehicleSet::button_1_0() { button_event(1, 0); }
void UCurrentVehicleSet::button_1_1() { button_event(1, 1); }
void UCurrentVehicleSet::button_1_2() { button_event(1, 2); }
void UCurrentVehicleSet::button_1_3() { button_event(1, 3); }
void UCurrentVehicleSet::button_1_4() { button_event(1, 4); }

void UCurrentVehicleSet::button_2_0() { button_event(2, 0); }
void UCurrentVehicleSet::button_2_1() { button_event(2, 1); }
void UCurrentVehicleSet::button_2_2() { button_event(2, 2); }
void UCurrentVehicleSet::button_2_3() { button_event(2, 3); }
void UCurrentVehicleSet::button_2_4() { button_event(2, 4); }

void UCurrentVehicleSet::button_3_0() { button_event(3, 0); }
void UCurrentVehicleSet::button_3_1() { button_event(3, 1); }
void UCurrentVehicleSet::button_3_2() { button_event(3, 2); }
void UCurrentVehicleSet::button_3_3() { button_event(3, 3); }
void UCurrentVehicleSet::button_3_4() { button_event(3, 4); }

void UCurrentVehicleSet::button_4_0() { button_event(4, 0); }
void UCurrentVehicleSet::button_4_1() { button_event(4, 1); }
void UCurrentVehicleSet::button_4_2() { button_event(4, 2); }
void UCurrentVehicleSet::button_4_3() { button_event(4, 3); }
void UCurrentVehicleSet::button_4_4() { button_event(4, 4); }

void UCurrentVehicleSet::button_5_0() { button_event(5, 0); }
void UCurrentVehicleSet::button_5_1() { button_event(5, 1); }
void UCurrentVehicleSet::button_5_2() { button_event(5, 2); }
void UCurrentVehicleSet::button_5_3() { button_event(5, 3); }
void UCurrentVehicleSet::button_5_4() { button_event(5, 4); }

void UCurrentVehicleSet::button_6_0() { button_event(6, 0); }
void UCurrentVehicleSet::button_6_1() { button_event(6, 1); }
void UCurrentVehicleSet::button_6_2() { button_event(6, 2); }
void UCurrentVehicleSet::button_6_3() { button_event(6, 3); }
void UCurrentVehicleSet::button_6_4() { button_event(6, 4); }
								
void UCurrentVehicleSet::button_7_0() { button_event(7, 0); }
void UCurrentVehicleSet::button_7_1() { button_event(7, 1); }
void UCurrentVehicleSet::button_7_2() { button_event(7, 2); }
void UCurrentVehicleSet::button_7_3() { button_event(7, 3); }
void UCurrentVehicleSet::button_7_4() { button_event(7, 4); }

void UCurrentVehicleSet::button_8_0() { button_event(8, 0); }
void UCurrentVehicleSet::button_8_1() { button_event(8, 1); }
void UCurrentVehicleSet::button_8_2() { button_event(8, 2); }
void UCurrentVehicleSet::button_8_3() { button_event(8, 3); }
void UCurrentVehicleSet::button_8_4() { button_event(8, 4); }

void UCurrentVehicleSet::button_9_0() { button_event(9, 0); }
void UCurrentVehicleSet::button_9_1() { button_event(9, 1); }
void UCurrentVehicleSet::button_9_2() { button_event(9, 2); }
void UCurrentVehicleSet::button_9_3() { button_event(9, 3); }
void UCurrentVehicleSet::button_9_4() { button_event(9, 4); }

void UCurrentVehicleSet::button_c_0() { color_event(0); }
void UCurrentVehicleSet::button_c_1() { color_event(1); }
void UCurrentVehicleSet::button_c_2() { color_event(2); }
void UCurrentVehicleSet::button_c_3() { color_event(3); }
void UCurrentVehicleSet::button_c_4() { color_event(4); }
void UCurrentVehicleSet::button_c_5() { color_event(5); }
void UCurrentVehicleSet::button_c_6() { color_event(6); }
void UCurrentVehicleSet::button_c_7() { color_event(7); }
void UCurrentVehicleSet::button_c_8() { color_event(8); }
void UCurrentVehicleSet::button_c_9() { color_event(9); }

void UCurrentVehicleSet::button_a_0() { ammo_event(0); }
void UCurrentVehicleSet::button_a_1() { ammo_event(1); }
void UCurrentVehicleSet::button_a_2() { ammo_event(2); }