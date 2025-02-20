-- simple use of the file selector dialog
--

import System (getArgs, ExitCode(ExitSuccess, ExitFailure), exitWith)
import Monad  (when)
import Gtk    (GtkWindowType(..),
               GtkObjectSignalHandler(..), gtkObjectSignalConnect,
	       GtkBox, gtkBoxPackStartDefaults,
	       GtkHBox, gtkHBoxNew,
	       GtkVBox, gtkVBoxNew,
	       GtkButton, gtkButtonNewWithLabel,
	       GtkButtonSignalHandler(..), gtkButtonSignalConnect,
	       gtkButtonSignalConnectObject,  
	       GtkContainer, gtkContainerBorderWidth, gtkContainerAdd,
	       GtkEditableSignalHandler(..), gtkEditableSignalConnect,
	       GtkEntry, gtkEntryGetText, gtkEntrySetVisibility, 
	       gtkEntrySetEditable, gtkEntryNewWithMaxLength, gtkEntrySetText,
	       gtkEntryAppendText, gtkEntrySelectRegion,
	       GtkFileSelection, gtkFileSelectionNew,
	       gtkFileSelectionGetFilename, gtkFileSelectionShowFileopButtons,
	       gtkFileSelectionQueryButtons, gtkFileSelectionSetFilename,
	       GtkLabel, gtkLabelNew,
	       GtkRadioButton, gtkRadioButtonNewWithLabel,
	       gtkRadioButtonNewWithLabelFromWidget, 
	       GtkWidget, gtkWidgetShow, gtkWidgetDestroy, gtkWidgetSetUSize,
	       GtkWidgetSignalHandler(..), gtkWidgetSignalConnect,
	       GtkWindow, gtkWindowNew, gtkWindowSetTitle,
	       gtkInit, gtkMain, gtkMainQuit)


enter     :: GtkEntry -> IO ()
enter ety  = do
	       text <- gtkEntryGetText ety
	       putStr ("Selected file: " ++ text ++ "\n")

-- create a file selection dialog that gets its initial filename from the
-- given entry and writes the result back to the entry
--
newFileSel     :: GtkEntry -> IO ()
newFileSel ety  = do
		    fsel <- gtkFileSelectionNew "Choose your file..." 
		    fname <- gtkEntryGetText ety
		    gtkFileSelectionSetFilename fsel fname
		    gtkFileSelectionShowFileopButtons fsel
		    (ok, cancel) <- gtkFileSelectionQueryButtons fsel
		    let okHdl = GtkButtonClickedHandler 
				  (const (storeFName ety fsel))
		    gtkButtonSignalConnect ok okHdl
		    let cancelHdl = GtkButtonClickedHandler 
				      (const (gtkWidgetDestroy fsel))
		    gtkButtonSignalConnect cancel cancelHdl
		    gtkWidgetShow fsel

storeFName          :: GtkEntry -> GtkFileSelection -> IO ()
storeFName ety fsel  = do
		         fname <- gtkFileSelectionGetFilename fsel
			 gtkEntrySetText ety fname
			 gtkWidgetDestroy fsel
                    
main :: IO ()
main  = do
          preArgs <- getArgs
	  args <- gtkInit preArgs
	  when (length args /= 1)
	    (putStr "Filename argument needed!\n" >> exitWith (ExitFailure 1))
	  let initialFileName = head args

	  window <- gtkWindowNew GtkWindowToplevel
	  gtkWindowSetTitle window "GTK+ File Selection Example"
	  gtkWidgetSetUSize window 300 180
	  let winDelHdl = GtkWidgetDeleteEventHandler (const gtkMainQuit)
	  gtkWidgetSignalConnect window winDelHdl
	  gtkContainerBorderWidth window 10

	  -- box for the whole thing
	  --
	  bigBox <- gtkVBoxNew False 10
	  gtkContainerAdd window bigBox
	  gtkWidgetShow bigBox

	  -- box for the radio buttons
	  --
	  rbutBox <- gtkVBoxNew False 0
	  gtkContainerAdd bigBox rbutBox
	  gtkWidgetShow rbutBox

	  -- the radio buttons
	  --
	  lbl <- gtkLabelNew "Choose your favorite passion:"
	  gtkContainerAdd rbutBox lbl
	  gtkWidgetShow lbl
	  rb1 <- gtkRadioButtonNewWithLabel "Linux rules!"
	  gtkContainerAdd rbutBox rb1
	  gtkWidgetShow rb1
	  rb2 <- gtkRadioButtonNewWithLabelFromWidget rb1 "Emacs rules!"
	  gtkContainerAdd rbutBox rb2
	  gtkWidgetShow rb2
	  rb3 <- gtkRadioButtonNewWithLabelFromWidget rb2 
	           "I am boring; I don't have a passion."
	  gtkContainerAdd rbutBox rb3
	  gtkWidgetShow rb3

	  -- hbox
	  --
	  hbox <- gtkHBoxNew False 10
	  gtkContainerAdd bigBox hbox
	  gtkWidgetShow hbox

	  entry <- gtkEntryNewWithMaxLength 100
	  let actHdl = GtkEditableActivateHandler enter
	  gtkEditableSignalConnect entry actHdl
	  gtkEntrySetText entry initialFileName
	  fullText <- gtkEntryGetText entry
	  gtkEntrySelectRegion entry 0 (length fullText)
	  gtkBoxPackStartDefaults hbox entry
	  gtkWidgetShow entry

	  -- button to activate file selection
	  --
	  fselBut <- gtkButtonNewWithLabel " Browse "
	  gtkBoxPackStartDefaults hbox fselBut
	  let fselHdl = GtkButtonClickedHandler (const (newFileSel entry))
	  gtkButtonSignalConnect fselBut fselHdl
	  gtkWidgetShow fselBut

	  buttonQuit <- gtkButtonNewWithLabel " Quit "
	  let quitHandler wd = gtkWidgetDestroy wd >> gtkMainQuit
	  gtkButtonSignalConnectObject 
	    buttonQuit (GtkButtonClickedHandler quitHandler) window
	  gtkBoxPackStartDefaults bigBox buttonQuit
	  gtkWidgetShow buttonQuit

	  gtkWidgetShow window

	  gtkMain
	  exitWith ExitSuccess
