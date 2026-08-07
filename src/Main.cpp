// Define in ONE file, this will be responsible for all Clay management
#define CLAY_IMPLEMENTATION
#include <clay.h>
#include <raylib/clay_renderer_raylib.c>

// Clay element presets
const int FONT_ID_BODY_16 = 0;

void BuildSpacer() {
	CLAY_AUTO_ID({
		.layout = { .sizing = { CLAY_SIZING_GROW(0) } }
	}) {}
}

void BuildHeaderButton(Clay_String text) {
	CLAY_AUTO_ID({
		.layout = {
			.padding = {16, 16, 8, 8}
		},
		.backgroundColor = { 140, 140, 140, 255 },
		.cornerRadius = 5
	}) {
		CLAY_TEXT(text, CLAY_TEXT_CONFIG({
			.textColor = { 255, 255, 255, 255 },
			.fontId = FONT_ID_BODY_16,
			.fontSize = 16
		}));
	}
}

struct Document {
	Clay_String title;
	Clay_String contents;
};

struct DocumentArray {
	Document* documents;
	uint32_t length;
};

uint32_t selectedDocumentIndex = 0;

DocumentArray documents = {
	.documents = new Document[] {
		{CLAY_STRING("Squirrels"), CLAY_STRING("They key is to have every key, the key to open every door. The key to success is to keep your head above the water, never give up. I'm up to something. Every chance I get, I water the plants, Lion! The key to more success is to have a lot of pillows. Life is what you make it, so let's make it. Always remember in the jungle there's a lot of they in there, after you overcome they, you will make it to paradise. It's on you how you want to live your life. Everyone has a choice. I pick my choice, squeaky clean.\n\nFind peace, life is like a water fall, you've gotta flow. Wraith talk. They don't want us to win. The first of the month is coming, we have to get money, we have no choice. It cost money to eat and they don't want you to eat. It's on you how you want to live your life. Everyone has a choice. I pick my choice, squeaky clean. The key to more success is to have a lot of pillows. In life you have to take the trash out, if you have trash in your life, take it out, throw it away, get rid of it, major key. Let's see what Chef Dee got that they don't want us to eat.\n\nEgg whites, turkey sausage, wheat toast, water. Of course they don't want us to eat our breakfast, so we are going to enjoy our breakfast. Don't ever play yourself. The key to more success is to get a massage once a week, very important, major key, cloth talk. Cloth talk. How's business? Boomin. You see that bamboo behind me though, you see that bamboo? Ain't nothin' like bamboo. Bless up.")},
		{CLAY_STRING("Lorem Ipsum"), CLAY_STRING("Find peace, life is like a water fall, you've gotta flow. We the best. Surround yourself with angels. Special cloth alert. The first of the month is coming, we have to get money, we have no choice. It cost money to eat and they don't want you to eat. Celebrate success right, the only way, apple. Egg whites, turkey sausage, wheat toast, water. Of course they don't want us to eat our breakfast, so we are going to enjoy our breakfast. Let me be clear, you have to make it through the jungle to make it to paradise, that's the key, Lion! They key is to have every key, the key to open every door.\n\nA major key, never panic. Don't panic, when it gets crazy and rough, don't panic, stay calm. The first of the month is coming, we have to get money, we have no choice. It cost money to eat and they don't want you to eat. In life you have to take the trash out, if you have trash in your life, take it out, throw it away, get rid of it, major key. Bless up. The key to success is to keep your head above the water, never give up. Fan luv. The oatmeal is a vibe, oatmeal is a major key!\n\nSurround yourself with angels, positive energy, beautiful people, beautiful souls, clean heart, angel. The other day the grass was brown, now it's green because I ain't give up. Never surrender. Give thanks to the most high. To be successful you've got to work hard, to make history, simple, you've got to make it. They don't want us to win. The key to more success is to have a lot of pillows. Surround yourself with angels, positive energy, beautiful people, beautiful souls, clean heart, angel.\n\nI told you all this before, when you have a swimming pool, do not use chlorine, use salt water, the healing, salt water is the healing. You should never complain, complaining is a weak emotion, you got life, we breathing, we blessed. The oatmeal is a vibe, oatmeal is a major key! You see that bamboo behind me though, you see that bamboo? Ain't nothin' like bamboo. Bless up. In life there will be road blocks but we will over come it. Special cloth alert.")},
		{CLAY_STRING("Vacuum Instructions"), CLAY_STRING("Lets see what Chef Dee got that they don't want us to eat. How's business? Boomin. Life is what you make it, so let's make it. We the best. Surround yourself with angels, positive energy, beautiful people, beautiful souls, clean heart, angel. To be successful you've got to work hard, to make history, simple, you've got to make it. Wraith talk. The key is to enjoy life, because they don't want you to enjoy life. I promise you, they don't want you to jetski, they don't want you to smile.\n\nPut it this way, it took me twenty five years to get these plants, twenty five years of blood sweat and tears, and I'm never giving up, I'm just getting started. You do know, you do know that they don't want you to have lunch. I'm keeping it real with you, so what you going do is have lunch. You should never complain, complaining is a weak emotion, you got life, we breathing, we blessed. To succeed you must believe. When you believe, you will succeed. You do know, you do know that they don't want you to have lunch. I'm keeping it real with you, so what you going do is have lunch.")},
		{CLAY_STRING("Article 4"), CLAY_STRING("You do know, you do know that they don't want you to have lunch. I'm keeping it real with you, so what you going do is have lunch. Mogul talk. The other day the grass was brown, now it's green because I ain't give up. Never surrender. Surround yourself with angels. Surround yourself with angels, positive energy, beautiful people, beautiful souls, clean heart, angel. We don't see them, we will never see them. Major key, don't fall for the trap, stay focused. It's the ones closest to you that want to see you fail.\n\nI'm giving you cloth talk, cloth. Special cloth alert, cut from a special cloth. It's important to use cocoa butter. It's the key to more success, why not live smooth? Why live rough? Major key, don't fall for the trap, stay focused. It's the ones closest to you that want to see you fail. I told you all this before, when you have a swimming pool, do not use chlorine, use salt water, the healing, salt water is the healing. Cloth talk. The key to more success is to get a massage once a week, very important, major key, cloth talk.\n\nMogul talk. Egg whites, turkey sausage, wheat toast, water. Of course they don't want us to eat our breakfast, so we are going to enjoy our breakfast. Let's see what Chef Dee got that they don't want us to eat. I'm giving you cloth talk, cloth. Special cloth alert, cut from a special cloth. The weather is amazing, walk with me through the pathway of more success. Take this journey with me, Lion! Wraith talk. I'm up to something. Watch your back, but more importantly when you get out the shower, dry your back, it's a cold world out there.")},
		{CLAY_STRING("Article 5"), CLAY_STRING("The ladies always say Khaled you smell good, I use no cologne. Cocoa butter is the key. Look at the sunset, life is amazing, life is beautiful, life is what you make it. To succeed you must believe. When you believe, you will succeed. Another one. Bless up. Always remember in the jungle there's a lot of they in there, after you overcome they, you will make it to paradise. I'm giving you cloth talk, cloth. Special cloth alert, cut from a special cloth.\n\nTo be successful you've got to work hard, to make history, simple, you've got to make it. I told you all this before, when you have a swimming pool, do not use chlorine, use salt water, the healing, salt water is the healing. Cloth talk. I'm giving you cloth talk, cloth. Special cloth alert, cut from a special cloth. Every chance I get, I water the plants, Lion! The ladies always say Khaled you smell good, I use no cologne. Cocoa butter is the key. Surround yourself with angels.\n\nThe other day the grass was brown, now it's green because I ain't give up. Never surrender. To succeed you must believe. When you believe, you will succeed. Every chance I get, I water the plants, Lion! Let me be clear, you have to make it through the jungle to make it to paradise, that's the key, Lion! Don't ever play yourself. Always remember in the jungle there's a lot of they in there, after you overcome they, you will make it to paradise.\n\nThey never said winning was easy. Some people can't handle success, I can. You smart, you loyal, you a genius. You see that bamboo behind me though, you see that bamboo? Ain't nothin' like bamboo. Bless up. The key is to drink coconut, fresh coconut, trust me. Always remember in the jungle there's a lot of they in there, after you overcome they, you will make it to paradise. Give thanks to the most high. To succeed you must believe. When you believe, you will succeed. Always remember in the jungle there's a lot of they in there, after you overcome they, you will make it to paradise.")}
	},
	.length = 5
};

// Basic Clay Error Handler
void HandleClayErrors(Clay_ErrorData errorData) {
	printf("%s -- %s", errorData.errorType, errorData.errorText.chars);
}

int main(void) {
	// DEBUG
	// Clay_SetDebugModeEnabled(true);

	// Raylib init
	Clay_Raylib_Initialize(1024, 768, "Encore DEV",
		FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_MSAA_4X_HINT | FLAG_WINDOW_HIGHDPI
	);
	
	// Clay init
	uint64_t clayRequiredMemory = Clay_MinMemorySize();
	Clay_Arena clayMemory = Clay_CreateArenaWithCapacityAndMemory(clayRequiredMemory, malloc(clayRequiredMemory));
	Clay_Dimensions screenDimensions = {
		.width = (float) GetScreenWidth(),
		.height = (float) GetScreenHeight()
	};
	Clay_ErrorHandler clayErrorHandler = {
		.errorHandlerFunction = HandleClayErrors
	};
	Clay_Initialize(clayMemory, screenDimensions, clayErrorHandler);


	Font fonts[1];
	fonts[FONT_ID_BODY_16] = LoadFontEx("resources/Montserrat-Regular.ttf", 16, NULL, 0);
	Clay_SetMeasureTextFunction(Raylib_MeasureText, fonts);

	// Main loop
	while (!WindowShouldClose()) {	// Raylib, detects close button or ESC key
		// Runs once per frame
		
		if (IsWindowResized) {
			// Update Clay's layout size 
			Clay_SetLayoutDimensions(Clay_Dimensions {
				.width = (float) GetScreenWidth(),
				.height = (float) GetScreenHeight()
			});
		}

		// Layout shorthand
		Clay_Sizing layoutExpand = {
			.width = CLAY_SIZING_GROW(0),
			.height = CLAY_SIZING_GROW(0)
		};

		Clay_Color GREY = {90, 90, 90, 255};
		Clay_Color COLOR_WHITE = { 255, 255, 255, 255 };

		float ROUNDING = 8.0f;

		// UI Construction
		Clay_BeginLayout();
		// Main background element
		CLAY( CLAY_ID("Background"),
		{
			.layout = {
				.sizing = layoutExpand,
				.padding = CLAY_PADDING_ALL(16),
				.childGap = (16),
				.layoutDirection = CLAY_TOP_TO_BOTTOM
			},
			.backgroundColor = {43, 41, 51, 255}
		}) {
			// Child elements go inside braces
			CLAY(
				CLAY_ID("Header_Bar"),
				{
					.layout = {
						.sizing = {
							.width = CLAY_SIZING_GROW(0),
							.height = CLAY_SIZING_FIXED(60)
						},
						.padding = CLAY_PADDING_ALL(16),
						.childGap = 16,
						.childAlignment = {
							.y = CLAY_ALIGN_Y_CENTER
						}
					},
					.backgroundColor = GREY,
					.cornerRadius = ROUNDING,
				}
			) {
				// Header buttons
				BuildHeaderButton(CLAY_STRING("File"));
				BuildHeaderButton(CLAY_STRING("Edit"));
				BuildSpacer();
				BuildHeaderButton(CLAY_STRING("Upload"));
				BuildHeaderButton(CLAY_STRING("Media"));
				BuildHeaderButton(CLAY_STRING("Support"));
			}

			CLAY(
				CLAY_ID("LowerContent"),
				{
					.layout = {
						.sizing = layoutExpand,
						.childGap = (16)
					}
				}
			) {

				CLAY(
					CLAY_ID("Sidebar"),
					{
						.layout = {
							.sizing = {
								.width = CLAY_SIZING_FIXED(250),
								.height = CLAY_SIZING_GROW(0)
							},
							.padding = CLAY_PADDING_ALL(16),
							.childGap = 8,
							.layoutDirection = CLAY_TOP_TO_BOTTOM
						},
						.backgroundColor = GREY,
						.cornerRadius = ROUNDING
					}
				) {
					// Document Titles
					for (int i = 0; i < documents.length; i++) {
						Document document = documents.documents[i];
						CLAY(
							CLAY_IDI("DocumentTitle", i), {
								.layout = {
									.padding = CLAY_PADDING_ALL(16)
								}
							}
						) {
							CLAY_TEXT(document.title, CLAY_TEXT_CONFIG({
								.textColor = COLOR_WHITE,
								.fontId = FONT_ID_BODY_16,
								.fontSize = 16
							}));
						}
					}
				}

				CLAY(
					CLAY_ID("MainContent"),
					{
						.layout = {
							.sizing = layoutExpand,
							.padding = CLAY_PADDING_ALL(16),
							.childGap = 16,
							.layoutDirection = CLAY_TOP_TO_BOTTOM
						},
						.backgroundColor = GREY,
						.cornerRadius = ROUNDING,
						.clip = {
							.vertical = true,
							.childOffset = Clay_GetScrollOffset() }
					}
				) {
					Document selectedDocument = documents.documents[selectedDocumentIndex];
					CLAY_TEXT(selectedDocument.title, CLAY_TEXT_CONFIG({
						.textColor = COLOR_WHITE,
						.fontId = FONT_ID_BODY_16,
						.fontSize = 24
					}));
					CLAY_TEXT(selectedDocument.contents, CLAY_TEXT_CONFIG({
						.textColor = COLOR_WHITE,
						.fontId = FONT_ID_BODY_16,
						.fontSize = 24
					}));
				}

			}
		};

		Clay_RenderCommandArray renderCommands = Clay_EndLayout(GetFrameTime());

		// UI 
		BeginDrawing();
		ClearBackground(BLACK);
		Clay_Raylib_Render(renderCommands, fonts);
		EndDrawing();

	};
	Clay_Raylib_Close();
	return 0;
}